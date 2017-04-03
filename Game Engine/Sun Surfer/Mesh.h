#ifndef MESH_H_
#define MESH_H_

#include "GL\glew.h"

#include "VertexFormat.h"
#include "ColorVertex.h"
#include "RenderInfo.h"

class Mesh
{
public:
	void*		 verts;
	void*		 indices;

	GLint		 numVerts;
	GLint		 numIndices;

	GLint		 shaderID;
	VertexFormat vertFormat;

	float width;
	float height;
	float depth;

	RenderInfo   renderInfo;

public:
	inline GLsizeiptr GetVertSizeInBytes()
	{
		return GetFloatsPerVert(vertFormat) * sizeof(GLfloat) * numVerts;
	}
	inline GLsizeiptr GetIndexSizeInBytes()
	{
		return sizeof(renderInfo.indexDataType) * numIndices;
	}
};
#endif // !MESH_H_
