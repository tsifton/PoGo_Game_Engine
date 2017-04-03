#ifndef OBJREADER_H_
#define OBJREADER_H_

#include <fstream>

#include "ConfigReader.h"
#include "MeshBuilder.h"
#include "BinaryWriter.h"

class ObjReader
{
public:
	ObjReader(ConfigReader* cfg) : m_cfg(cfg), m_meshBuilder(cfg) {};

public:
	bool Initialize(const char* filename);
	bool Shutdown();

	bool AnalyzeFile();
	bool ProcessFile();

	bool BuildSceneFile();

private:
	bool OpenFile(std::ifstream& stream);
	bool CloseFile(std::ifstream& stream);

	bool AnalyzeLine(const char* line);
	bool ProcessLine(const char* line);

	bool HasPositionData(const char* line);
	bool ExtractPositionData(std::istringstream& stream);

	bool HasTextureData(const char* line);
	bool ExtractTextureData(std::istringstream& stream);

	bool HasNormalData(const char* line);
	bool ExtractNormalData(std::istringstream& stream);

	bool HasIndexData(const char* line);
	bool ExtractIndexData(std::istringstream& stream);

	void IgnoreDivider(std::istringstream& stream);

private:
	std::ifstream m_ifs;
	const char* m_modelKey;
	const char* m_objFilename;

	int m_positionCounter = 0;
	int m_texCoordCounter = 0;
	int m_normalCounter = 0;
	int m_faceCounter = 0;

	MeshBuilder m_meshBuilder;
	BinaryWriter m_sceneWriter;

	ConfigReader* m_cfg;
};
#endif // !OBJREADER_H_
