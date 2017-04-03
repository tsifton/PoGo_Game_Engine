#ifndef MESHBUILDER_H_
#define MESHBUILDER_H_

#include "GL\glew.h"
#include "ConfigReader.h"
#include "Mesh.h"

class MeshBuilder
{
public:
	MeshBuilder(ConfigReader* cfg) : m_cfg(cfg) {};

public:
	bool Initialize(const char* modelKey);
	bool Shutdown();

	void AddPosition(float v1, float v2, float v3);
	void AddColor(float c1, float c2, float c3);
	void AddTexCoord(float t1, float t2);
	void AddNormal(float n1, float n2, float n3);

	void AddPositionIndex(GLuint index);
	void AddColorIndex(GLuint index);
	void AddTexCoordIndex(GLuint index);
	void AddNormalIndex(GLuint index);

	bool  BuildMesh();
	Mesh* GetMesh();

private:
	bool  SetVertexFormat();
	bool  SetNumVerts();
	bool  SetNumTexCoords();
	bool  SetNumNormals();
	bool  SetNumIndices();
	bool  SetColor();
	bool  SetRenderInfo();
	bool  CreateDataBuffers();

	void InterleavePosition(int indexIntoVertArray, int indexIntoIndexArray);
	void InterleaveColor(int indexIntoVertArray);
	void InterleaveTexCoord(int indexIntoVertArray, int indexIntoIndexArray);
	void InterleaveNormal(int indexIntoVertArray, int indexIntoIndexArray);

	float GetRandomColor();

private:
	Mesh m_mesh;
	const char* m_modelKey;

	int m_numTexCoords;
	int m_numNormals;
	int m_numFaces;
	int m_numIndices;

	float*	   m_verts{ nullptr };
	GLuint*	   m_indices{ nullptr };
	
	glm::vec3* m_positions{ nullptr };
	glm::vec2* m_texCoords{ nullptr };
	glm::vec3* m_normals{ nullptr };
	glm::vec3* m_colors{ nullptr };

	bool m_customColorSet;
	glm::vec3 m_customColor;

	ConfigReader* m_cfg;
};
#endif // !MESHBUILDER_H_
