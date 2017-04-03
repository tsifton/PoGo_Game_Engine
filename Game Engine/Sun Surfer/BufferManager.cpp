#include "BufferManager.h"
#include "GameLogger.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "MyGL.h"

int BufferManager::bufferGroupCounter = 0;
BufferGroup BufferManager::m_bufferGroups[MAXBUFFERGROUPS];

bool BufferManager::Initialize()
{
	GameLogger::Log(MsgType::Process, "BufferManager::Initialize() successful.\n");
	return true;
}

bool BufferManager::Shutdown()
{
	GameLogger::Log(MsgType::Process, "BufferManager::Shutdown() successful.\n");
	return true;
}

bool BufferManager::AddMesh(Mesh * mesh)
{
	GLint meshShaderID = mesh->shaderID;
	VertexFormat meshVertFormat = mesh->vertFormat;

	/*
	* Loop through each buffer group. If you find a group that has the same Shader ID
	* and Vertex Format, add the mesh to that group.
	*/
	for (int i = 0; i <= bufferGroupCounter; i++)
	{
		BufferGroup& bufferGroup = m_bufferGroups[i];
		if (bufferGroup.shaderID == meshShaderID && bufferGroup.vertFormat == meshVertFormat)
		{
			bufferGroup.AddMesh(mesh);
			return true;
		}
	}

	/* 
	* If it doesn't match a group, intialize a new one and add the mesh to that group.
	*/
	BufferGroup& newBufferGroup = m_bufferGroups[bufferGroupCounter++];
	newBufferGroup.Initialize(meshShaderID, meshVertFormat);
	newBufferGroup.AddMesh(mesh);
	return true;
}

bool BufferManager::AddGraphicalObject(GraphicsObject * gob)
{
	GLuint vertBufferID = gob->mesh->renderInfo.vertBufferID;
	for (int i = 0; i < bufferGroupCounter; i++)
	{
		BufferGroup& bufferGroup = m_bufferGroups[i];
		if (bufferGroup.AddGraphicalObject(gob)) return true;
	}
	GameLogger::Log(MsgType::Error, "BufferManager::AddGraphicalObject() - Unable to add graphical object to buffer with ID: %d\n", vertBufferID);
	return false;
}

BufferGroup* BufferManager::GetBufferGroups()
{
	return m_bufferGroups;
}

int BufferManager::GetNumBufferGroups()
{
	return bufferGroupCounter;
}

