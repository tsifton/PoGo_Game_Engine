#ifndef BUFFERMANAGER_H_
#define BUFFERMANAGER_H_

#include "GL\glew.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

#include "BufferGroup.h"

class Mesh;
struct ColorVertex;
class GraphicsObject;
class BufferManager
{
public:
	static bool Initialize();
	static bool Shutdown();

	static bool AddMesh(Mesh* mesh);
	static bool AddGraphicalObject(GraphicsObject* gob);

	static BufferGroup* GetBufferGroups();
	static int		    GetNumBufferGroups();

private:
	static const int MAXBUFFERGROUPS = 100;
	static BufferGroup m_bufferGroups[MAXBUFFERGROUPS];
	static int bufferGroupCounter;

};
#endif // !BUFFERMANAGER_H_
