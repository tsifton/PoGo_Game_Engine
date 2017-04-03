#include "Buffer.h"

#include "MyGL.h"
#include "Mesh.h"
#include "GraphicsObject.h"

bool Buffer::Initialize(VertexFormat format, int sizeOfInitialVertsInBytes, int sizeOfInitialIndicesInBytes)
{
	glGenBuffers(1, &vertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeOfInitialVertsInBytes < MINBUFFERSIZE ? MINBUFFERSIZE : sizeOfInitialVertsInBytes, NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfInitialIndicesInBytes < MINBUFFERSIZE ? MINBUFFERSIZE : sizeOfInitialIndicesInBytes, NULL, GL_STATIC_DRAW);

	InitializeVertexArrayObject(format);
	return initialized = (!MyGL::TestForError(MsgType::cFatalError, "Buffer::Initialize() - Unable to successfully create buffers.\n"));
}

bool Buffer::InitializeVertexArrayObject(VertexFormat vertFormat)
{
	glGenVertexArrays(1, &vertArrayObjectID);
	
	glBindVertexArray(vertArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	SetAttributes(vertFormat);
	glBindVertexArray(0);

	return true;
}

bool Buffer::AddMesh(Mesh * mesh)
{
	GLsizeiptr vertSizeInBytes = mesh->GetVertSizeInBytes();
	if (!BufferVertexData(vertSizeInBytes, mesh->verts)) return false;
	SetVertexRenderInfo(mesh);

	GLsizeiptr indexSizeInBytes = mesh->GetIndexSizeInBytes();
	if (!BufferIndexData(indexSizeInBytes, mesh->indices)) return false;
	SetIndexRenderInfo(mesh);

	SetOffsetValues(vertSizeInBytes, mesh->numVerts, indexSizeInBytes);
	return true;
}

bool Buffer::AddGraphicalObject(GraphicsObject * gob)
{
	gob->nextNode = headNode;
	headNode = gob;
	return true;
}

bool Buffer::HasSpace(int sizeOfVertsInBytes, int sizeOfIndicesInBytes)
{
	return (sizeOfVertsInBytes + vertOffsetInBytes < MINBUFFERSIZE) && (sizeOfIndicesInBytes + indexOffsetInBytes < MINBUFFERSIZE);
}

bool Buffer::BufferVertexData(GLsizeiptr vertSizeInBytes, void* verts)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, vertOffsetInBytes, vertSizeInBytes, verts);
	return (!MyGL::TestForError(MsgType::cFatalError, "Buffer::BufferVertexData() - Unable to successfully buffer vertex data.\n"));
}

bool Buffer::BufferIndexData(GLsizeiptr indexSizeInBytes, void* indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexOffsetInBytes, indexSizeInBytes, indices);
	return (!MyGL::TestForError(MsgType::cFatalError, "Buffer::BufferIndexData() - Unable to successfully buffer index data.\n"));
}

void Buffer::SetVertexRenderInfo(Mesh * mesh)
{
	RenderInfo& info = mesh->renderInfo;
	//printf("----- Buffer::SetVertexRenderInfo() ----- \nVertex Buffer ID: %d\nNumber of Vertices: %d verts\nVertices' Size: %d bytes\nVertices' Offset Into Buffer: %d verts\nVertices' Offset Into Buffer: %d bytes\n\n", vertBufferID, mesh->numVerts, mesh->GetVertSizeInBytes(), vertOffset, vertOffsetInBytes);
	info.vertOffsetInBytes = vertOffsetInBytes;
	info.vertOffset = vertOffset;
	info.vertBufferID = vertBufferID;
}

void Buffer::SetIndexRenderInfo(Mesh * mesh)
{
	RenderInfo& info = mesh->renderInfo;
	//printf("----- Buffer::SetIndexRenderInfo() ----- \nIndex Buffer ID: %d\nNumber of Indices: %d indices\nIndices' Size: %d bytes\nIndices' Offset Into Buffer: %d bytes\n\n", indexBufferID, mesh->numIndices, mesh->GetIndexSizeInBytes(), indexOffsetInBytes);
	info.indexOffsetInBytes = indexOffsetInBytes;
	info.indexBufferID = indexBufferID;
}

void Buffer::SetOffsetValues(GLsizeiptr vertSizeInBytes, GLint numVerts, GLsizeiptr indexSizeInBytes)
{
	vertOffsetInBytes += vertSizeInBytes;
	vertOffset += numVerts;
	indexOffsetInBytes += indexSizeInBytes;
}

bool Buffer::SetAttributes(VertexFormat vertFormat)
{
	const GLuint floatsPerPosition = GetFloatsPerType(VertexFormat::P);
	const GLuint floatsPerColor = GetFloatsPerType(VertexFormat::C);
	const GLuint floatsPerTexCoord = GetFloatsPerType(VertexFormat::T);
	const GLuint floatsPerNormal = GetFloatsPerType(VertexFormat::N);

	const GLsizei vertStride = GetFloatsPerVert(vertFormat) * sizeof(GL_FLOAT);
	GLuint attribIndex = 0;
	GLuint formatOffset = 0;

	// NOTE: attribIndex and formatOffset are modified by SetAttrib()!!!
	if (vertFormat & VertexFormat::HasPosition) SetAttrib(attribIndex, floatsPerPosition, vertStride, formatOffset);
	if (vertFormat & VertexFormat::HasColor)    SetAttrib(attribIndex, floatsPerColor, vertStride, formatOffset);
	if (vertFormat & VertexFormat::HasTexture)  SetAttrib(attribIndex, floatsPerTexCoord, vertStride, formatOffset);
	if (vertFormat & VertexFormat::HasNormal)   SetAttrib(attribIndex, floatsPerNormal, vertStride, formatOffset);
	return true;
}

bool Buffer::SetAttrib(GLuint & attribIndex, GLint floatsPerDataType, GLsizei vertStride, GLuint & formatOffset)
{
	const GLboolean isNormalized = GL_FALSE;
	const GLenum    dataType = GL_FLOAT;

	glEnableVertexAttribArray(attribIndex);
	glVertexAttribPointer(attribIndex, floatsPerDataType, dataType, isNormalized, vertStride, (GLvoid*)formatOffset);
	formatOffset += floatsPerDataType * sizeof(dataType); // move over the data to the next (example: move over the position to get to the color)
	++attribIndex;										  // move to the next attrib

	return (!MyGL::TestForError(MsgType::cError, "Buffer::SetAttrib() - Unable to successfully enable and set vertex attrib pointer for attrib index [%d].\n", attribIndex));
}