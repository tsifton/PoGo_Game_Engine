#include "SceneLoader.h"

#include <cstring>

int SceneLoader::numScenes;
LoadedScene SceneLoader::m_scenes[MAX_SCENE_FILES];

bool SceneLoader::GetMeshFromSceneFile(const char * filepath, Mesh *& mesh)
{
	// Attempt to retrieve already loaded mesh
	if (!GetLoadedMesh(filepath, mesh))
	{
		// If the mesh hasn't already been loaded, load it
		return LoadMeshFromSceneFile(filepath, mesh);
	}
	return true;
}

bool SceneLoader::GetLoadedMesh(const char * filepath, Mesh *& mesh)
{
	for (int i = 0; i < numScenes; i++)
	{
		LoadedScene scene = m_scenes[i];
		if (strcmp(scene.filepath, filepath) == 0)
		{
			mesh = scene.mesh;
			return true;
		}
	}
	GameLogger::Log(MsgType::Info, "SceneLoader::GetLoadedMesh() - Unable to find a scene already loaded with the filepath [%s].\n", filepath);
	return false;
}

bool SceneLoader::LoadMeshFromSceneFile(const char * filepath, Mesh *& mesh)
{
	std::ifstream inputStream(filepath, std::ios::binary | std::ios::in);
	if (!inputStream)
	{
		GameLogger::Log(MsgType::cError, "SceneLoader::LoadMesh() - Unable to open binary input file [%s]\n", filepath);
		return false;
	}

	int dataLen;
	inputStream.read(reinterpret_cast<char*>(&dataLen), sizeof(dataLen));
	//printf("Reading %d bytes from (%s)\n\n", dataLen, filepath);

	// TODO: CHECK TO SEE IF DATALEN IS <= ZERO
	char* data = new char[dataLen];
	assert(data);
	memset(data, 0, dataLen);

	inputStream.read(data, dataLen);
	inputStream.close();

	Mesh* sceneMesh = reinterpret_cast<Mesh*>(data);

	char* p = data;
	p += sizeof(*sceneMesh);
	float* verts = reinterpret_cast<float*>(p);
	sceneMesh->verts = verts;
	p += sceneMesh->GetVertSizeInBytes();
	sceneMesh->indices = reinterpret_cast<GLuint*>(p);

	float maxWidth = -INFINITY, minWidth = INFINITY;
	float maxHeight = -INFINITY, minHeight = INFINITY;
	float maxDepth = -INFINITY, minDepth = INFINITY;

	int floatsPerVert = GetFloatsPerVert(sceneMesh->vertFormat);
	int numFloats = sceneMesh->numVerts * floatsPerVert;
	for (int i = 0; i < numFloats; i += floatsPerVert)
	{
		if (verts[i] > maxWidth) maxWidth = verts[i];
		if (verts[i] < minWidth) minWidth = verts[i];
		if (verts[i + 1] > maxHeight) maxHeight = verts[i + 1];
		if (verts[i + 1] < minHeight) minHeight = verts[i + 1];
		if (verts[i + 2] > maxDepth) maxDepth = verts[i + 2];
		if (verts[i + 2] < minDepth) minDepth = verts[i + 2];
	}

	sceneMesh->width = maxWidth - minWidth;
	sceneMesh->height = maxHeight - minHeight;
	sceneMesh->depth = maxDepth - minDepth;

	mesh = sceneMesh;
	m_scenes[numScenes++] = { filepath, sceneMesh };
	return true;
}