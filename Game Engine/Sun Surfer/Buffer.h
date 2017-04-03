#ifndef BUFFER_H_
#define BUFFER_H_

#include "GL\glew.h"
#include "VertexFormat.h"

class Mesh;
class GraphicsObject;
class Buffer
{
public:
	bool Initialize(VertexFormat format, int sizeOfInitialVertsInBytes, int sizeOfInitialIndicesInBytes);

	bool AddMesh(Mesh* mesh);
	bool AddGraphicalObject(GraphicsObject* gob);

	bool HasSpace(int sizeOfVertsInBytes, int sizeOfIndicesInBytes);

private:
	bool InitializeVertexArrayObject(VertexFormat vertFormat);
	
	bool BufferVertexData(GLsizeiptr vertSizeInBytes, void* verts);
	bool BufferIndexData(GLsizeiptr indexSizeInBytes, void* indices);
	
	void SetVertexRenderInfo(Mesh* mesh);
	void SetIndexRenderInfo(Mesh* mesh);
	void SetOffsetValues(GLsizeiptr vertSizeInBytes, GLint numVerts, GLsizeiptr indexSizeInBytes);
	
	bool SetAttributes(VertexFormat vertFormat);
	bool SetAttrib(GLuint & attribIndex, GLint floatsPerDataType, GLsizei vertStride, GLuint& formatOffset);

public:
	bool initialized = false;

	static const int MINBUFFERSIZE = 1000000;

	GLuint vertArrayObjectID;
	GLuint vertBufferID;
	GLuint indexBufferID;

	GLintptr   vertOffset;
	GLsizeiptr vertOffsetInBytes;
	GLsizeiptr indexOffsetInBytes;

	GraphicsObject* headNode;
};
#endif // !BUFFER_H_
