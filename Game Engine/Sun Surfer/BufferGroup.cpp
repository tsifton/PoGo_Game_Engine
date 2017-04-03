#include "BufferGroup.h"

#include "GraphicsObject.h"
#include "Mesh.h"


bool BufferGroup::Initialize(GLint shader, VertexFormat format)
{
	shaderID = shader;
	vertFormat = format;
	return true;
}

bool BufferGroup::AddMesh(Mesh * mesh)
{
	int meshVertSizeInBytes = mesh->GetVertSizeInBytes();
	int meshIndicesSizeInBytes = mesh->GetIndexSizeInBytes();

	/*
	* Loop through each buffer. If you find a buffer that is intialized and
	* has enough space for the mesh, add the mesh to that buffer.
	*/
	for (int i = 0; i <= bufferCounter; i++)
	{
		Buffer& buffer = m_buffers[i];
		if (buffer.initialized && buffer.HasSpace(meshVertSizeInBytes, meshIndicesSizeInBytes))
		{
			buffer.AddMesh(mesh);
			return true;
		}
	}

	/*
	* If there are no buffers that are intialized or have enough space, initialize
	* a new buffer and add the mesh to it.
	*/
	Buffer& newBuffer = m_buffers[bufferCounter++];
	newBuffer.Initialize(mesh->vertFormat, meshVertSizeInBytes, meshIndicesSizeInBytes);
	newBuffer.AddMesh(mesh);
	return true;
}

bool BufferGroup::AddGraphicalObject(GraphicsObject * gob)
{
	/*
	* Loop through each buffer. If you find a buffer that has a
	* bufferID that matches, add the Graphical Object to it.
	*/
	GLuint vertBufferID = gob->mesh->renderInfo.vertBufferID;
	for (int j = 0; j < bufferCounter; j++)
	{
		Buffer& buffer = m_buffers[j];
		if (vertBufferID == buffer.vertBufferID)
		{
			buffer.AddGraphicalObject(gob);
			return true;
		}
	}
	return false;
}
