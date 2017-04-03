#ifndef SCENELOADER_H_
#define SCENELOADER_H_

#include "Mesh.h"

struct LoadedScene
{
	const char* filepath;
	Mesh* mesh;
};

class SceneLoader
{
public:
	static bool GetMeshFromSceneFile(const char* filepath, Mesh*& mesh);
	static bool GetLoadedMesh(const char* filepath, Mesh*& mesh);
	static bool LoadMeshFromSceneFile(const char* filepath, Mesh*& mesh);

private:
	static int numScenes;
	static const int MAX_SCENE_FILES = 200;
	static LoadedScene m_scenes[MAX_SCENE_FILES];
};
#endif // !SCENELOADER_H_
