#include "ObjReader.h"
#include "GameLogger.h"

#include <sstream>

#include <ctime>
#include <cstdlib>
#include <iostream>

static int numLines = 0;

bool ObjReader::Initialize(const char * modelKey)
{
	m_modelKey = modelKey;
	if (!m_meshBuilder.Initialize(modelKey)) return false;
	if (!OpenFile(m_ifs)) return false;
	GameLogger::Log(MsgType::Process, "ObjReader::Initialize() successful.\n");
	return true;
}

bool ObjReader::Shutdown()
{
	if (!CloseFile(m_ifs)) return false;
	if (!m_meshBuilder.Shutdown()) return false;
	GameLogger::Log(MsgType::Process, "ObjReader::Shutdown() successful.\n");
	return true;
}

bool ObjReader::OpenFile(std::ifstream & stream)
{
	char filenameKey[100];
	strcpy_s(filenameKey, m_modelKey);
	strcat_s(filenameKey, ".Filename");
	const char* filename;
	if (!m_cfg->GetStringForKey(filenameKey, filename)) return false;
	stream.open(filename);
	if (!stream.is_open())
	{
		GameLogger::Log(MsgType::cFatalError, "ObjReader::OpenFile() - Unable to open file [%s]\n", filename);
		return false;
	}
	m_objFilename = filename;
	return true;
}

bool ObjReader::CloseFile(std::ifstream & stream)
{
	if (stream.is_open()) stream.close();
	return true;
}

bool ObjReader::BuildSceneFile()
{
	if (!m_meshBuilder.BuildMesh()) return false;
	char outputKey[100];
	strcpy_s(outputKey, m_modelKey);
	strcat_s(outputKey, ".OutputFilename");
	const char* filename;
	if (!m_cfg->GetStringForKey(outputKey, filename)) return false;;
	return m_sceneWriter.WriteCustomBinaryFile(filename, m_meshBuilder.GetMesh());
}

bool ObjReader::AnalyzeFile()
{
	clock_t startTime = clock();
	const int lineSize = 512;
	char line[lineSize];
	while (m_ifs.getline(line, lineSize))
	{
		if (!AnalyzeLine(line))
		{
			CloseFile(m_ifs);
			return false;
		}
	}
	double elapsedTime = (clock() - startTime) / (double)CLOCKS_PER_SEC;
	GameLogger::Log(MsgType::cInfo, "ObjReader::AnalyzeFile() -\n\nAnalyzed File [%s]\nFile Analyzing Time: %f seconds\nNumber of Vertices: %d\nNumber of TexCoords: %d\nNumber of Normals: %d\nNumber of Faces: %d\n\n", m_objFilename, elapsedTime, m_positionCounter, m_texCoordCounter, m_normalCounter, m_faceCounter);
	return true;
}

bool ObjReader::AnalyzeLine(const char * line)
{
	if (line[0] == 'v' && line[1] == ' ') m_positionCounter++;
	if (line[0] == 'v' && line[1] == 't') m_texCoordCounter++;
	if (line[0] == 'v' && line[1] == 'n') m_normalCounter++;
	if (line[0] == 'f')					  m_faceCounter++;
	return true;
}

bool ObjReader::ProcessFile()
{
	clock_t startTime = clock();
	const int lineSize = 512;
	char line[lineSize];
	while (m_ifs.getline(line, lineSize))
	{
		if (!ProcessLine(line))
		{
			CloseFile(m_ifs);
			return false;
		}
	}
	double elapsedTime = (clock() - startTime) / (double)CLOCKS_PER_SEC;
	GameLogger::Log(MsgType::cInfo, "ObjReader::ProcessFile() -\n\nProcessed File [%s]\nFile Processing Time: %f seconds\nLines Processed: %d\n\n", m_objFilename, elapsedTime, numLines);
	return true;
}

bool ObjReader::ProcessLine(const char* line)
{
	numLines++;
	/*printf("Line: %d - ", numLines);*/
	if (HasPositionData(line))
	{
		std::istringstream parser(line += 2);
		ExtractPositionData(parser);
		return true;
	}

	if (HasTextureData(line))
	{
		std::istringstream parser(line += 2);
		ExtractTextureData(parser);
		return true;
	}

	if (HasNormalData(line))
	{
		std::istringstream parser(line += 2);
		ExtractNormalData(parser);
		return true;
	}

	if (HasIndexData(line))
	{
		std::istringstream parser(line += 2);
		ExtractIndexData(parser);
		return true;
	}
	/*std::cout << std::endl;*/
	return true;
}

bool ObjReader::HasPositionData(const char * line)
{
	return (line[0] == 'v' && line[1] == ' ');
}

bool ObjReader::ExtractPositionData(std::istringstream& stream)
{
	float verts[3];
	for (int i = 0; i < 3; i++)
	{
		stream >> verts[i];
		/*std::cout << verts[i] << " ";*/
	}
	m_meshBuilder.AddPosition(verts[0], verts[1], verts[2]);
	/*std::cout << std::endl;*/
	return true;
}

bool ObjReader::HasTextureData(const char * line)
{
	return (line[0] == 'v' && line[1] == 't');
}

bool ObjReader::ExtractTextureData(std::istringstream & stream)
{
	float texCoord[2];
	for (int i = 0; i < 2; i++)
	{
		stream >> texCoord[i];
		/*std::cout << texCoord[i] << " ";*/
	}
	m_meshBuilder.AddTexCoord(texCoord[0], texCoord[1]);
	/*std::cout << std::endl;*/
	return true;
}

bool ObjReader::HasNormalData(const char * line)
{
	return (line[0] == 'v' && line[1] == 'n');
}

bool ObjReader::ExtractNormalData(std::istringstream & stream)
{
	float normal[3];
	for (int i = 0; i < 3; i++)
	{
		stream >> normal[i];
		/*std::cout << normal[i] << " ";*/
	}
	m_meshBuilder.AddNormal(normal[0], normal[1], normal[2]);
	/*std::cout << std::endl;*/
	return true;
}

bool ObjReader::HasIndexData(const char * line)
{
	return (line[0] == 'f');
}

bool ObjReader::ExtractIndexData(std::istringstream & stream)
{
	// get the position index that will always be at the beginning
	GLuint posIndex;
	while (stream >> posIndex)
	{
		m_meshBuilder.AddPositionIndex(posIndex);
		/*std::cout << posIndex;*/

		//ignore the '/' divider
		IgnoreDivider(stream);

		// attempt to get texture index that may or may not be there
		if (stream.peek() != '/' && stream.peek() != ' ')
		{
			GLuint texIndex;
			if (stream >> texIndex) 
			{ 
				m_meshBuilder.AddTexCoordIndex(texIndex);
				/*std::cout << "/" << texIndex; */
			};
		}

		//ignore the '/' divider
		IgnoreDivider(stream);

		// attempt to get normal index that may or may not be there
		if (stream.peek() != '/' && stream.peek() != ' ')
		{
			GLuint normIndex;
			if (stream >> normIndex) 
			{
				m_meshBuilder.AddNormalIndex(normIndex);
				/*std::cout << "/" << normIndex;*/
			};
		}
		/*std::cout << " ";*/
	}
	/*std::cout << std::endl;*/
	return true;
}

void ObjReader::IgnoreDivider(std::istringstream & stream)
{
	if (stream.peek() == '/') stream.ignore(1);
}