#ifndef BUFFERGROUP_H_
#define BUFFERGROUP_H_

#include "GL\glew.h"
#include "VertexFormat.h"
#include "Buffer.h"

class Mesh;
class GraphicsObject;
struct BufferGroup
{
public:
	bool Initialize(GLint shader, VertexFormat format);

	bool AddMesh(Mesh* mesh);
	bool AddGraphicalObject(GraphicsObject* gob);

public:
	GLint       shaderID;
	VertexFormat vertFormat;

	static const int MAXBUFFERS = 100;
	Buffer m_buffers[MAXBUFFERS];
	int bufferCounter = 0;
};
#endif // !BUFFERGROUP_H_
