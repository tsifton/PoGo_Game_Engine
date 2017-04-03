#ifndef COLORVERTEX_H_
#define COLORVERTEX_H_

#include "GL\glew.h"

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#pragma warning(pop)

struct ColorVertex
{
public:
	glm::vec3 m_position;
	glm::vec3 m_color;

public:
	inline static GLenum GetDataType()
	{
		return GL_FLOAT;
	}
	inline static void* GetColorOffset()
	{
		return (void*)sizeof(m_position);
	}
	inline static void* GetPositionOffset()
	{
		return (void*)0;
	}
	inline static unsigned GetPositionCount()
	{
		return 3;
	}
	inline static unsigned GetColorCount()
	{
		return 3;
	}
	inline static unsigned GetStride()
	{
		return sizeof(m_position) + sizeof(m_color);
	}
};
#endif // !COLORVERTEX_H_
