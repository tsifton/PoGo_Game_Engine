#ifndef BINARYWRITER_H_
#define BINARYWRITER_H_

#include "GL\glew.h"

#include "Mesh.h"
#include "VertexFormat.h"

#include <fstream>

class BinaryWriter
{
public:
	bool WriteCustomBinaryFile(const char* filename, Mesh* pMesh);

private:
	int WriteMeshData	(std::ofstream& out, Mesh* pMesh);
	int WriteVertData	(std::ofstream& out, VertexFormat format, int numVerts, void* pVerts);
	int WriteIndexData	(std::ofstream& out, IndexDataType type, int numIndices, void* pIndices);
	int WriteInt		(std::ofstream& out, int value);
};
#endif // !BINARYWRITER_H_
