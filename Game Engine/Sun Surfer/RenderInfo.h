#ifndef RENDERINFO_H_
#define RENDERINFO_H_

#include "GL\glew.h"

enum class IndexDataType
{
	Ubyte = GL_UNSIGNED_BYTE,     // one  byte indices
	Ushort = GL_UNSIGNED_SHORT,     // two  byte indices
	Uint = GL_UNSIGNED_INT,     // four byte indices
};

enum class ModelSize
{
	Small = 256,   // use Ubyte for 256 indices or fewer
	Medium = 65536, // use Ushort for 257-65536 indices
	Large = 65537  // use Uint for more than 65536 indices 
};

struct RenderInfo
{
	GLenum	mode;

	GLuint vertBufferID;
	GLuint indexBufferID;

	IndexDataType indexDataType;
	GLsizeiptr	  indexOffsetInBytes;

	GLuint		vertOffset;
	GLsizeiptr	vertOffsetInBytes;
};
#endif // !RENDERINFO_H_
