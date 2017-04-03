#ifndef VERTEXFORMAT_H_
#define VERTEXFORMAT_H_

#include <cstring>
#include "GameLogger.h"

enum class VertexFormat
{
	HasPosition = 1,
	HasColor = 2,
	HasTexture = 4,
	HasNormal = 8,
	P = HasPosition,
	C = HasColor,
	T = HasTexture,
	N = HasNormal,
	PC = HasPosition | HasColor,
	PT = HasPosition | HasTexture,
	PN = HasPosition | HasNormal,
	PCN = HasPosition | HasColor | HasNormal,
	PTN = HasPosition | HasTexture | HasNormal,
	PCT = HasPosition | HasColor | HasTexture,
	PCTN = HasPosition | HasColor | HasTexture | HasNormal,
	UNKNOWN = -1
};

inline int operator&(VertexFormat left, VertexFormat right)
{
	return static_cast<int>(left) & static_cast<int>(right);
}

inline int GetFloatsPerType(VertexFormat format)
{
	switch (format)
	{
	case VertexFormat::P:
		return 3;
	case VertexFormat::C:
		return 3;
	case VertexFormat::T:
		return 2;
	case VertexFormat::N:
		return 3;
	default:
		return -1;
	}
}

inline int GetFloatsPerVert(VertexFormat format)
{
	int numFloats = 0;
	if (format & VertexFormat::HasPosition) numFloats += GetFloatsPerType(VertexFormat::P);
	if (format & VertexFormat::HasColor) numFloats += GetFloatsPerType(VertexFormat::C);
	if (format & VertexFormat::HasTexture) numFloats += GetFloatsPerType(VertexFormat::T);
	if (format & VertexFormat::HasNormal) numFloats += GetFloatsPerType(VertexFormat::N);
	return numFloats;
}

inline int GetNumIndicesPerVert(VertexFormat format)
{
	int numIndices = 0;
	if (format & VertexFormat::HasPosition) numIndices += 1;
	if (format & VertexFormat::HasTexture) numIndices += 1;
	if (format & VertexFormat::HasNormal) numIndices += 1;
	return numIndices;
}

inline int GetNumIndicesPerFace(VertexFormat format)
{
	return GetNumIndicesPerVert(format) * 3;
}

inline VertexFormat GetFormatFromString(const char* formatString)
{
	if (strcmp(formatString, "P") == 0)		return VertexFormat::P;
	if (strcmp(formatString, "PT") == 0)	return VertexFormat::PT;
	if (strcmp(formatString, "PN") == 0)	return VertexFormat::PN;
	if (strcmp(formatString, "PC") == 0)	return VertexFormat::PC;
	if (strcmp(formatString, "PTN") == 0)	return VertexFormat::PTN;
	if (strcmp(formatString, "PCT") == 0)	return VertexFormat::PCT;
	if (strcmp(formatString, "PCN") == 0)	return VertexFormat::PCN;
	if (strcmp(formatString, "PCTN") == 0)	return VertexFormat::PCTN;
	return VertexFormat::UNKNOWN;
}

inline int GetPositionInVertIndex(VertexFormat vertFormat, VertexFormat type)
{
	int posCounter = 0;
	if (vertFormat & VertexFormat::HasPosition)
	{
		if (type == VertexFormat::P) return posCounter;
		posCounter++;
	}
	if (vertFormat & VertexFormat::HasColor)
	{
		if (type == VertexFormat::C) return posCounter;
		posCounter++;
	}
	if (vertFormat & VertexFormat::HasTexture)
	{
		if (type == VertexFormat::T) return posCounter;
		posCounter++;
	}
	if (vertFormat & VertexFormat::HasNormal)
	{
		if (type == VertexFormat::N) return posCounter;
		posCounter++;
	}
	return -1;
}
#endif // !VERTEXFORMAT_H_