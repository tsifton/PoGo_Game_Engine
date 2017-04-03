#include "MeshBuilder.h"

#include <iostream>
#include "GameLogger.h"

#pragma warning(push)
#pragma warning(disable :4201)
#include "gtc\random.hpp"
#pragma warning(pop)

IndexDataType GetIndexSize(int numIndices)
{
	if (numIndices <= static_cast<int>(ModelSize::Small))
		return IndexDataType::Ubyte;
	if (numIndices <= static_cast<int>(ModelSize::Medium))
		return IndexDataType::Ushort;
	return IndexDataType::Uint;
}

bool MeshBuilder::Initialize(const char* modelKey)
{
	m_modelKey = modelKey;
	if (!SetVertexFormat())		return false;
	if (!SetNumVerts())			return false;
	if (!SetNumTexCoords())		return false;
	if (!SetNumNormals())		return false;
	if (!SetNumIndices())		return false;
	if (!SetColor())			return false;
	if (!SetRenderInfo())		return false;
	if (!CreateDataBuffers())	return false;
	return true;
}

bool MeshBuilder::Shutdown()
{
	if (m_positions)
	{
		delete[] m_positions;
		m_positions = nullptr;
	}
	if (m_colors)
	{
		delete[] m_colors;
		m_colors = nullptr;
	}
	if (m_texCoords)
	{
		delete[] m_texCoords;
		m_texCoords = nullptr;
	}
	if (m_normals)
	{
		delete[] m_normals;
		m_normals = nullptr;
	}
	return true;
}

void MeshBuilder::AddPosition(float v1, float v2, float v3)
{
	static unsigned int posCounter = 0;
	if (m_mesh.vertFormat & VertexFormat::HasPosition) m_positions[posCounter++] = glm::vec3(v1, v2, v3);
}

void MeshBuilder::AddColor(float c1, float c2, float c3)
{
	static unsigned int colorCounter = 0;
	if (m_mesh.vertFormat & VertexFormat::HasColor) m_colors[colorCounter++] = glm::vec3(c1, c2, c3);
}

void MeshBuilder::AddTexCoord(float t1, float t2)
{
	static unsigned int texCounter = 0;
	if (m_mesh.vertFormat & VertexFormat::HasTexture) m_texCoords[texCounter++] = glm::vec2(t1, t2);
}

void MeshBuilder::AddNormal(float n1, float n2, float n3)
{
	static unsigned int normalCounter = 0;
	if (m_mesh.vertFormat & VertexFormat::HasNormal) m_normals[normalCounter++] = glm::vec3(n1, n2, n3);
}

static unsigned int m_indexCounter = 0;
// subtract 1 because .obj format starts at 1 whereas the OpenGl buffer starts at 0
void MeshBuilder::AddPositionIndex(GLuint index)
{
	if (m_mesh.vertFormat & VertexFormat::HasPosition) m_indices[m_indexCounter++] = index - 1;
}

void MeshBuilder::AddColorIndex(GLuint index)
{
	if (m_mesh.vertFormat & VertexFormat::HasColor) m_indices[m_indexCounter++] = index - 1;
}

void MeshBuilder::AddTexCoordIndex(GLuint index)
{
	if (m_mesh.vertFormat & VertexFormat::HasTexture) m_indices[m_indexCounter++] = index - 1;
}

void MeshBuilder::AddNormalIndex(GLuint index)
{
	if (m_mesh.vertFormat & VertexFormat::HasNormal) m_indices[m_indexCounter++] = index - 1;
}

bool MeshBuilder::BuildMesh()
{
	int numVertsPerFace = 3;
	m_verts = new float[m_numFaces * numVertsPerFace * GetFloatsPerVert(m_mesh.vertFormat)];

	VertexFormat vertFormat = m_mesh.vertFormat;
	int posInVertIndex = GetPositionInVertIndex(vertFormat, VertexFormat::P);
	int colorPosInVertIndex = GetPositionInVertIndex(vertFormat, VertexFormat::C);
	int texCoordPosInVertIndex = GetPositionInVertIndex(vertFormat, VertexFormat::T);
	int normalPosInVertIndex = GetPositionInVertIndex(vertFormat, VertexFormat::N);

	int numIndicesPerVert = GetNumIndicesPerVert(m_mesh.vertFormat) + 1;
	int floatCounter = 0;
	int indexCounter = 0;

	for (int i = 0; i < m_numFaces; i++)
	{
		/*printf("Face %d - ", i);*/
		for (int j = 0; j < numVertsPerFace; j++)
		{
			for (int k = 0; k < numIndicesPerVert; k++)
			{
				if (k == posInVertIndex)
				{
					InterleavePosition(floatCounter, indexCounter);
					floatCounter += GetFloatsPerType(VertexFormat::P);
					indexCounter++;
				}
				if (k == colorPosInVertIndex)
				{
					InterleaveColor(floatCounter);
					floatCounter += GetFloatsPerType(VertexFormat::C);
				}
				if (k == texCoordPosInVertIndex)
				{
					InterleaveTexCoord(floatCounter, indexCounter);
					floatCounter += GetFloatsPerType(VertexFormat::T);
					indexCounter++;
				}
				if (k == normalPosInVertIndex)
				{
					InterleaveNormal(floatCounter, indexCounter);
					floatCounter += GetFloatsPerType(VertexFormat::N);
					indexCounter++;
				}
			}
		}
		/*std::cout << std::endl;*/
	}

	m_mesh.verts = m_verts;
	m_mesh.indices = nullptr;
	m_mesh.numVerts = m_numFaces * 3;
	m_mesh.numIndices = 0;
	return true;
}

Mesh * MeshBuilder::GetMesh()
{
	return &m_mesh;
}

bool MeshBuilder::SetVertexFormat()
{
	char vertFormatKey[100];
	strcpy_s(vertFormatKey, m_modelKey);
	strcat_s(vertFormatKey, ".Format");
	const char* vertFormat;
	if (!m_cfg->GetStringForKey(vertFormatKey, vertFormat)) return false;
	m_mesh.vertFormat = GetFormatFromString(vertFormat);
	if (m_mesh.vertFormat == VertexFormat::UNKNOWN)
	{
		GameLogger::Log(MsgType::cFatalError, "MeshBuilder::SetVertexFormat() - Attempting to use an unsupported VertexFormat of [%s].\n", vertFormat);
		return false;
	}
	return true;
}

bool MeshBuilder::SetNumVerts()
{
	char numVertsKey[100];
	strcpy_s(numVertsKey, m_modelKey);
	strcat_s(numVertsKey, ".NumVerts");
	if (!m_cfg->GetIntForKey(numVertsKey, m_mesh.numVerts)) return false;
	return true;
}

bool MeshBuilder::SetNumTexCoords()
{
	char numTexCoordsKey[100];
	strcpy_s(numTexCoordsKey, m_modelKey);
	strcat_s(numTexCoordsKey, ".NumTexCoords");
	if (!m_cfg->GetIntForKey(numTexCoordsKey, m_numTexCoords)) return false;
	return true;
}

bool MeshBuilder::SetNumNormals()
{
	char numNormalsKey[100];
	strcpy_s(numNormalsKey, m_modelKey);
	strcat_s(numNormalsKey, ".NumNormals");
	if (!m_cfg->GetIntForKey(numNormalsKey, m_numNormals)) return false;
	return true;
}

bool MeshBuilder::SetNumIndices()
{
	char numIndicesKey[100];
	strcpy_s(numIndicesKey, m_modelKey);
	strcat_s(numIndicesKey, ".NumFaces");
	if (!m_cfg->GetIntForKey(numIndicesKey, m_numFaces)) return false;
	return true;
}

bool MeshBuilder::SetColor()
{
	char colorKey[100];
	strcpy_s(colorKey, m_modelKey);
	strcat_s(colorKey, ".Color");
	m_customColorSet = m_cfg->GetFloatsForKey(colorKey, 3, &m_customColor.r);
	return true;
}

bool MeshBuilder::SetRenderInfo()
{
	m_mesh.renderInfo.mode = GL_TRIANGLES;
	m_mesh.renderInfo.indexDataType = IndexDataType::Uint;
	return true;
}

bool MeshBuilder::CreateDataBuffers()
{
	m_numIndices = GetNumIndicesPerFace(m_mesh.vertFormat) * m_numFaces;
	m_indices = new GLuint[m_numIndices];
	if (m_mesh.vertFormat & VertexFormat::HasPosition) m_positions = new glm::vec3[m_mesh.numVerts];
	if (m_mesh.vertFormat & VertexFormat::HasColor)    m_colors = new glm::vec3[m_mesh.numVerts];
	if (m_mesh.vertFormat & VertexFormat::HasTexture)  m_texCoords = new glm::vec2[m_numTexCoords];
	if (m_mesh.vertFormat & VertexFormat::HasNormal)   m_normals = new glm::vec3[m_numNormals];
	return true;
}

void MeshBuilder::InterleavePosition(int indexIntoVertArray, int indexIntoIndexArray)
{
	int indexIntoPositionArray = m_indices[indexIntoIndexArray];
	/*printf("%d/", indexIntoPositionArray);*/
	glm::vec3 position = m_positions[indexIntoPositionArray];
	m_verts[indexIntoVertArray] = position.x;
	m_verts[indexIntoVertArray + 1] = position.y;
	m_verts[indexIntoVertArray + 2] = position.z;
}

void MeshBuilder::InterleaveColor(int indexIntoVertArray)
{
	glm::vec3 color = m_customColorSet ? m_customColor : glm::vec3(GetRandomColor(), GetRandomColor(), GetRandomColor());
	m_verts[indexIntoVertArray] = color.r;
	m_verts[indexIntoVertArray + 1] = color.g;
	m_verts[indexIntoVertArray + 2] = color.b;
}

void MeshBuilder::InterleaveTexCoord(int indexIntoVertArray, int indexIntoIndexArray)
{
	int indexIntoTexCoordArray = m_indices[indexIntoIndexArray];
	/*printf("%d/", indexIntoTexCoordArray);*/
	glm::vec2 texCoord = m_texCoords[indexIntoTexCoordArray];
	m_verts[indexIntoVertArray] = texCoord.x;
	m_verts[indexIntoVertArray + 1] = texCoord.y;
}

void MeshBuilder::InterleaveNormal(int indexIntoVertArray, int indexIntoIndexArray)
{
	int indexIntoNormalArray = m_indices[indexIntoIndexArray];
	/*printf("%d ", indexIntoNormalArray);*/
	glm::vec3 normal = m_normals[indexIntoNormalArray];
	m_verts[indexIntoVertArray] = normal.x;
	m_verts[indexIntoVertArray + 1] = normal.y;
	m_verts[indexIntoVertArray + 2] = normal.z;
}

float MeshBuilder::GetRandomColor()
{
	return glm::linearRand(0.0f, 1.0f);
}
