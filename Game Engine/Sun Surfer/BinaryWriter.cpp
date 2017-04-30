#include "BinaryWriter.h"
#include "GameLogger.h"
#include "Mesh.h"


bool BinaryWriter::WriteCustomBinaryFile(const char * filename, Mesh * pMesh)
{
	printf("----- (%s) -----\n", filename);

	int totalBytes = 0;
	std::ofstream outputStream(filename, std::ios::binary | std::ios::out);
	if (!outputStream)
	{
		GameLogger::Log(MsgType::cError, "BinaryWriter::WriteCustomBinaryFile() - Unable to open binary output file (%s)\n", filename);
		return false;
	}

	outputStream.seekp(0);
	WriteInt(outputStream, 0);

	Mesh mesh = *pMesh;
	/*memset(&mesh.renderInfo, 0, sizeof(mesh.renderInfo));*/
	
	totalBytes += WriteMeshData(outputStream, &mesh);
	totalBytes += WriteVertData(outputStream, mesh.vertFormat, mesh.numVerts, mesh.verts);
	totalBytes += WriteIndexData(outputStream, mesh.renderInfo.indexDataType, mesh.numIndices, mesh.indices);

	outputStream.seekp(0);
	WriteInt(outputStream, totalBytes);

	outputStream.close();
	printf("Total : Wrote %d bytes\n", totalBytes);

	printf("----- (%s) -----\n\n", filename);
	return true;
}

int BinaryWriter::WriteMeshData(std::ofstream & out, Mesh * pMesh)
{
	int size = sizeof(*pMesh);
	out.write(reinterpret_cast<char*>(pMesh), size);
	printf("Mesh : Wrote %d bytes\n", size);
	return size;
}

int BinaryWriter::WriteVertData(std::ofstream & out, VertexFormat format, int numVerts, void * pVerts)
{
	int size = GetFloatsPerVert(format) * sizeof(GL_FLOAT) * numVerts;
	out.write(reinterpret_cast<char*>(pVerts), size);
	printf("Verts : Wrote %d bytes\n", size);
	return size;
}

int BinaryWriter::WriteIndexData(std::ofstream & out, IndexDataType type, int numIndices, void * pIndices)
{
	int typeInBytes = sizeof(static_cast<int>(type));
	int size = typeInBytes * numIndices;
	out.write(reinterpret_cast<char*>(pIndices), size);
	printf("Indices : Wrote %d bytes (Bytes per index: %d)\n", size, typeInBytes);
	return size;
}

int BinaryWriter::WriteInt(std::ofstream & out, int value)
{
	int size = sizeof(value);
	out.write(reinterpret_cast<char*>(&value), size);
	printf("Header : Wrote %d bytes\n", size);
	return size;
}
