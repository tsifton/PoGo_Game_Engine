#include "ShapeGenerator.h"
#include "ColorVertex.h"
#include "GraphicsObject.h"
#include "RenderEngine.h"
#include "GameLogger.h"
#include "UniformCallback.h"

#pragma warning(push)
#pragma warning(disable :4201)
#include "gtc\random.hpp"
#pragma warning(pop)


const int numCubeVerts = 24;
ColorVertex cubeVerts[numCubeVerts] =
{
	// front
	glm::vec3(+.50f, +.50f, +.50f), glm::vec3(), glm::vec3(-.50f, +.50f, +.50f), glm::vec3(),
	glm::vec3(-.50f, -.50f, +.50f), glm::vec3(), glm::vec3(+.50f, -.50f, +.50f), glm::vec3(),

	// right
	glm::vec3(+.50f, +.50f, -.50f), glm::vec3(), glm::vec3(+.50f, +.50f, +.50f), glm::vec3(),
	glm::vec3(+.50f, -.50f, +.50f), glm::vec3(), glm::vec3(+.50f, -.50f, -.50f), glm::vec3(),

	// back
	glm::vec3(-.50f, +.50f, -.50f), glm::vec3(), glm::vec3(+.50f, +.50f, -.50f), glm::vec3(),
	glm::vec3(+.50f, -.50f, -.50f), glm::vec3(), glm::vec3(-.50f, -.50f, -.50f), glm::vec3(),

	// left
	glm::vec3(-.50f, +.50f, +.50f), glm::vec3(), glm::vec3(-.50f, +.50f, -.50f), glm::vec3(),
	glm::vec3(-.50f, -.50f, -.50f), glm::vec3(), glm::vec3(-.50f, -.50f, +.50f), glm::vec3(),

	// top
	glm::vec3(+.50f, +.50f, -.50f), glm::vec3(), glm::vec3(-.50f, +.50f, -.50f), glm::vec3(),
	glm::vec3(-.50f, +.50f, +.50f), glm::vec3(), glm::vec3(+.50f, +.50f, +.50f), glm::vec3(),

	// bottom
	glm::vec3(+.50f, -.50f, +.50f), glm::vec3(), glm::vec3(-.50f, -.50f, +.50f), glm::vec3(),
	glm::vec3(-.50f, -.50f, -.50f), glm::vec3(), glm::vec3(+.50f, -.50f, -.50f), glm::vec3(),
	
};

const int numCubeIndices = 36;
GLuint cubeIndices[numCubeIndices] =
{
	// front
	0, 1, 2, 2, 3, 0,
	// right
	4, 5, 6, 6, 7, 4,
	// back
	8, 9, 10, 10, 11, 8,
	// left
	12, 13, 14, 14, 15, 12,
	// top
	16, 17, 18, 18, 19, 16,
	// bottom
	20, 21, 22, 22, 23, 20
};

//glm::vec3 debugCubeColor = glm::vec3(0.0f, 1.0f, 0.0);
const int numDebugCubeVerts = 24;
glm::vec3 debugCubeVerts[numCubeVerts] =
{
	// front
	glm::vec3(+.50f, +.50f, +.50f), glm::vec3(-.50f, +.50f, +.50f),
	glm::vec3(-.50f, -.50f, +.50f), glm::vec3(+.50f, -.50f, +.50f),
								
	// right												
	glm::vec3(+.50f, +.50f, -.50f), glm::vec3(+.50f, +.50f, +.50f),
	glm::vec3(+.50f, -.50f, +.50f), glm::vec3(+.50f, -.50f, -.50f),
								
	// back													
	glm::vec3(-.50f, +.50f, -.50f), glm::vec3(+.50f, +.50f, -.50f),
	glm::vec3(+.50f, -.50f, -.50f), glm::vec3(-.50f, -.50f, -.50f),
								
	// left													
	glm::vec3(-.50f, +.50f, +.50f), glm::vec3(-.50f, +.50f, -.50f),
	glm::vec3(-.50f, -.50f, -.50f), glm::vec3(-.50f, -.50f, +.50f),
								
	// top													
	glm::vec3(+.50f, +.50f, -.50f), glm::vec3(-.50f, +.50f, -.50f),
	glm::vec3(-.50f, +.50f, +.50f), glm::vec3(+.50f, +.50f, +.50f),
								
	// bottom												
	glm::vec3(+.50f, -.50f, +.50f), glm::vec3(-.50f, -.50f, +.50f),
	glm::vec3(-.50f, -.50f, -.50f), glm::vec3(+.50f, -.50f, -.50f),

};

const int numDebugCubeIndices = 26;
GLuint debugCubeIndices[numDebugCubeIndices] =
{
	0, 1, 1, 2, 2, 3, 3, 0,
	6, 7, 7, 4, 4, 5,
	10, 11, 11, 8, 8, 9,
	14, 15, 15, 12, 12, 13
};

const glm::vec3 debugArrowColor = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 debugArrowColor2 = glm::vec3(0.0f, 0.0f, 1.0f);
const int numDebugArrowVerts = 42;
ColorVertex debugArrowVerts[numDebugArrowVerts] =
{
	glm::vec3(+0.50f, +0.25f, +0.25f), debugArrowColor, //0
	glm::vec3(-1.00f, +0.25f, +0.25f), debugArrowColor, //1
	glm::vec3(-1.00f, -0.25f, +0.25f), debugArrowColor, //2
	glm::vec3(+0.50f, -0.25f, +0.25f), debugArrowColor, //3

	glm::vec3(+0.50f, +0.25f, +0.50f), debugArrowColor, //4
	glm::vec3(+0.50f, +0.25f, +0.25f), debugArrowColor, //5
	glm::vec3(+0.50f, -0.25f, +0.25f), debugArrowColor, //6
	glm::vec3(+0.50f, -0.25f, +0.50f), debugArrowColor, //7

	glm::vec3(+1.00f, +0.25f, +0.00f), debugArrowColor, //8
	glm::vec3(+0.50f, +0.25f, +0.50f), debugArrowColor, //9
	glm::vec3(+0.50f, -0.25f, +0.50f), debugArrowColor, //10
	glm::vec3(+1.00f, -0.25f, +0.00f), debugArrowColor, //11

	glm::vec3(+0.50f, +0.25f, -0.50f), debugArrowColor, //12
	glm::vec3(+1.00f, +0.25f, +0.00f), debugArrowColor, //13
	glm::vec3(+1.00f, -0.25f, +0.00f), debugArrowColor, //14
	glm::vec3(+0.50f, -0.25f, -0.50f), debugArrowColor, //15

	glm::vec3(+0.50f, +0.25f, -0.25f), debugArrowColor, //16
	glm::vec3(+0.50f, +0.25f, -0.50f), debugArrowColor, //17
	glm::vec3(+0.50f, -0.25f, -0.50f), debugArrowColor, //18
	glm::vec3(+0.50f, -0.25f, -0.25f), debugArrowColor, //19

	glm::vec3(-1.00f, +0.25f, -0.25f), debugArrowColor, //20
	glm::vec3(+0.50f, +0.25f, -0.25f), debugArrowColor, //21
	glm::vec3(+0.50f, -0.25f, -0.25f), debugArrowColor, //22
	glm::vec3(-1.00f, -0.25f, -0.25f), debugArrowColor, //23

	glm::vec3(-1.00f, +0.25f, +0.25f), debugArrowColor, //24
	glm::vec3(-1.00f, +0.25f, -0.25f), debugArrowColor, //25
	glm::vec3(-1.00f, -0.25f, -0.25f), debugArrowColor, //26
	glm::vec3(-1.00f, -0.25f, +0.25f), debugArrowColor, //27

	glm::vec3(+0.50f, +0.25f, -0.25f), debugArrowColor2, //28
	glm::vec3(-1.00f, +0.25f, -0.25f), debugArrowColor2, //29
	glm::vec3(-1.00f, +0.25f, +0.25f), debugArrowColor2, //30
	glm::vec3(+0.50f, +0.25f, +0.25f), debugArrowColor2, //31

	glm::vec3(+0.50f, +0.25f, -0.50f), debugArrowColor2, //32
	glm::vec3(+0.50f, +0.25f, +0.50f), debugArrowColor2, //33
	glm::vec3(+1.00f, +0.25f, +0.00f), debugArrowColor2, //34

	glm::vec3(+0.50f, -0.25f, +0.25f), debugArrowColor2, //35
	glm::vec3(-1.00f, -0.25f, +0.25f), debugArrowColor2, //36
	glm::vec3(-1.00f, -0.25f, -0.25f), debugArrowColor2, //37
	glm::vec3(+0.50f, -0.25f, -0.25f), debugArrowColor2, //38

	glm::vec3(+0.50f, -0.25f, +0.50f), debugArrowColor2, //39
	glm::vec3(+0.50f, -0.25f, -0.50f), debugArrowColor2, //40
	glm::vec3(+1.00f, -0.25f, +0.00f), debugArrowColor2, //41
};

const int numDebugArrowIndices = 60;
GLuint debugArrowIndices[numDebugArrowIndices] =
{
	0, 1, 2, 2, 3, 0,		// face 1
	4, 5, 6, 6, 7, 4,		// face 2
	8, 9, 10, 10, 11, 8,	// face 3
	12, 13, 14, 14, 15, 12, // face 4
	16, 17, 18, 18, 19, 16, // face 5
	20, 21, 22, 22, 23, 20, // face 6
	24, 25, 26, 26, 27, 24, // face 7
	28, 29, 30, 30, 31, 28, // face 8
	32, 33, 34,				// face 9
	35, 36, 37, 37, 38, 35, // face 10
	39, 40, 41				// face 11
};

const int numPyramidVerts = 4;
ColorVertex pyramidVerts[numPyramidVerts] =
{
	glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(),
	glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec3(),
	glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec3(),
	glm::vec3(+0.0f, -1.0f, -1.0f), glm::vec3()
};

const int numPyramidIndices = 12;
GLuint pyramidIndices[numPyramidIndices] =
{
	// face 1
	0, 1, 2,
	// face 2
	0, 2, 3,
	// face 3
	0, 3, 1,
	// face 4
	1, 3, 2
};

const int maxGridSize = 10000;
int numGridVerts;
glm::vec3 gridVerts[maxGridSize + 1 * 4];

const int numPlaneVerts = 6;
glm::vec3 planeVerts[numPlaneVerts * 2] =
{
	glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f),
};

Mesh ShapeGenerator::m_cubeMesh;
Mesh ShapeGenerator::m_debugCubeMesh;
Mesh ShapeGenerator::m_pyramidMesh;
Mesh ShapeGenerator::m_debugArrowMesh;
Mesh ShapeGenerator::m_gridMesh;
Mesh ShapeGenerator::m_planeMesh;
Mesh ShapeGenerator::m_billboardMesh;
GraphicsObject* ShapeGenerator::m_points;

bool ShapeGenerator::Initialize()
{
	GameLogger::Log(MsgType::Process, "ShapeGenerator::Initialize() successful.\n");
	return true;
}

bool ShapeGenerator::Shutdown()
{
	GameLogger::Log(MsgType::Process, "ShapeGenerator::Shutdown() successful.\n");
	return true;
}

void ShapeGenerator::MakeCube(GraphicsObject* gob)
{
	gob->shaderID = Shader::PassThrough;
	SetRandomColors(cubeVerts, numCubeVerts);
	gob->mesh = GenerateMesh(&m_cubeMesh, Shader::PassThrough, VertexFormat::PC, GL_TRIANGLES, IndexDataType::Uint, cubeVerts, numCubeVerts, cubeIndices, numCubeIndices);
}

void ShapeGenerator::MakeDebugCube(GraphicsObject * gob, glm::vec3 color)
{
	gob->shaderID = Shader::Debug;
	gob->SetMaterialEmissiveColor(color);
	gob->mesh = GenerateMesh(&m_debugCubeMesh, Shader::Debug, VertexFormat::P, GL_LINES, IndexDataType::Uint, debugCubeVerts, numDebugCubeVerts, debugCubeIndices, numDebugCubeIndices);
}

void ShapeGenerator::MakePyramid(GraphicsObject * gob)
{
	gob->shaderID = Shader::PassThrough;
	SetRandomColors(pyramidVerts, numPyramidVerts);
	gob->mesh = GenerateMesh(&m_pyramidMesh, Shader::PassThrough, VertexFormat::PC, GL_TRIANGLES, IndexDataType::Uint, pyramidVerts, numPyramidVerts, pyramidIndices, numPyramidIndices);
}

void ShapeGenerator::MakeDebugArrow(GraphicsObject * gob)
{
	gob->shaderID = Shader::PassThrough;
	gob->mesh = GenerateMesh(&m_debugArrowMesh, Shader::PassThrough, VertexFormat::PC, GL_TRIANGLES, IndexDataType::Uint, debugArrowVerts, numDebugArrowVerts, debugArrowIndices, numDebugArrowIndices);
}

enum SphereDefines
{
	NUMSPHEREITERATIONS = 2,  // if you change this, you must change the next one too!!!
	FOUR_TO_NUM_ITERATIONS = 16,     // 4^numIterations, ie, 4 to the power of
	NUMSPHEREFACETS = FOUR_TO_NUM_ITERATIONS * 8, // 4^numIterations * 8
	NUMSPHEREVERTS = 3 * NUMSPHEREFACETS, // 3 verts per facet/triangle
	NUMSPHEREINDICES = 2 * NUMSPHEREVERTS   // two indices per point
};

Mesh ShapeGenerator::m_debugSphereMesh;
glm::vec3	sphereVerts[NUMSPHEREVERTS];
Facet3      sphereFacets[NUMSPHEREFACETS];
GLuint		sphereIndices[NUMSPHEREINDICES];

void ShapeGenerator::MakeDebugSphere(GraphicsObject * gob, glm::vec3 color)
{
	gob->shaderID = Shader::Debug;
	BuildSphere();
	gob->SetMaterialEmissiveColor(color);
	gob->mesh = GenerateMesh(&m_debugSphereMesh, Shader::Debug, VertexFormat::P, GL_LINES, IndexDataType::Uint, sphereVerts, NUMSPHEREVERTS, sphereIndices, NUMSPHEREINDICES);
}

void ShapeGenerator::MakeGrid(GraphicsObject * gob, int size, glm::vec3 color)
{
	if (!m_gridMesh.verts)
	{
		GenerateGrid(size);
	}
	gob->SetMaterialEmissiveColor(color);
	gob->mesh = GenerateMesh(&m_gridMesh, Shader::Debug, VertexFormat::P, GL_LINES, IndexDataType::Uint, gridVerts, numGridVerts);
}

void ShapeGenerator::MakePlane(GraphicsObject* gob, GLuint shaderID, glm::vec2 upperLeft, glm::vec2 lowerRight, float k, glm::vec3 color)
{
	if (!m_planeMesh.verts)
	{
		GeneratePlane(upperLeft, lowerRight, k);
	}
	gob->SetMaterialEmissiveColor(color);
	gob->mesh = GenerateMesh(&m_planeMesh, shaderID, VertexFormat::PN, GL_TRIANGLES, IndexDataType::Uint, planeVerts, numPlaneVerts);
}

void ShapeGenerator::MakePoints(GraphicsObject* gob, GLuint shaderID, int numPoints, glm::vec3 upperLeftBound, glm::vec3 lowerRightBound)
{
	glm::vec3* pointVerts = new glm::vec3[numPoints];
	for (int i = 0; i < numPoints; i++)
	{
		pointVerts[i] = glm::vec3(glm::linearRand(upperLeftBound.x, lowerRightBound.x), glm::linearRand(upperLeftBound.y, lowerRightBound.y), glm::linearRand(upperLeftBound.z, lowerRightBound.z));
	}
	gob->mesh = GenerateMesh(&m_billboardMesh, shaderID, VertexFormat::P, GL_POINTS, IndexDataType::Uint, pointVerts, numPoints);
}

// TODO: MAKE MORE EFFICIENT!!!! IT'S BAD AT THE MOMENT!
void ShapeGenerator::GenerateGrid(int size)
{
	if (size > maxGridSize) size = maxGridSize;
	float s = 1.0f / size;
	for (int i = 0; i <= size; i++)
	{
		for (int j = 0; j <= size; j++)
		{
			if (i == 0)
			{
				gridVerts[numGridVerts++] = glm::vec3(j * s - 0.5f, 0.0f, i * s - 0.5f);
				gridVerts[numGridVerts++] = glm::vec3(j * s - 0.5f, 0.0f, (i + size) * s - 0.5f);
			}
			if (j == 0)
			{
				gridVerts[numGridVerts++] = glm::vec3(j * s - 0.5f, 0.0f, i * s - 0.5f);
				gridVerts[numGridVerts++] = glm::vec3((j + size) * s - 0.5f, 0.0f, i * s - 0.5f);
			}
		}
	}
}

void ShapeGenerator::GeneratePlane(glm::vec2 upperLeft, glm::vec2 lowerRight, float k)
{
	planeVerts[0] = glm::vec3(upperLeft.x, k, -upperLeft.y);
	planeVerts[2] = glm::vec3(upperLeft.x, k, -lowerRight.y);
	planeVerts[4] = glm::vec3(upperLeft.y, k, -lowerRight.x);
	planeVerts[6] = glm::vec3(upperLeft.y, k, -lowerRight.x);
	planeVerts[8] = glm::vec3(upperLeft.x, k, -lowerRight.y);
	planeVerts[10] = glm::vec3(lowerRight.x, k, -lowerRight.y);
}

Mesh* ShapeGenerator::GenerateMesh(Mesh * mesh, GLuint shaderID, VertexFormat vertFormat, GLenum mode, IndexDataType indexDataType, void * verts, int numVerts)
{
	if (!mesh->verts)
	{
		SetMeshValues(mesh, shaderID, vertFormat, mode, indexDataType, verts, numVerts, nullptr, 0);
		RenderEngine::AddMesh(mesh);
	}
	return mesh;
}

Mesh* ShapeGenerator::GenerateMesh(Mesh* mesh, GLuint shaderID, VertexFormat vertFormat, GLenum mode, IndexDataType indexDataType, void* verts, int numVerts, GLuint* indices, int numIndices)
{
	if (!mesh->verts)
	{
		SetMeshValues(mesh, shaderID, vertFormat, mode, indexDataType, verts, numVerts, indices, numIndices);
		RenderEngine::AddMesh(mesh);
	}
	return mesh;
}

void ShapeGenerator::SetMeshValues(Mesh* mesh, GLuint shaderID, VertexFormat vertFormat, GLenum mode, IndexDataType indexDataType, void* verts, int numVerts, GLuint* indices, int numIndices)
{
	mesh->shaderID	 = shaderID;
	mesh->vertFormat = vertFormat;

	mesh->verts	   = verts;
	mesh->numVerts = numVerts;

	mesh->indices    = indices;
	mesh->numIndices = numIndices;

	mesh->renderInfo.mode = mode;
	mesh->renderInfo.indexDataType = indexDataType;
}

void ShapeGenerator::SetRandomColors(ColorVertex* verts, int numVerts)
{
	for (int i = 0; i < numVerts; i++)
	{
		for (unsigned j = 0; j < ColorVertex::GetColorCount(); j++)
		{
			verts[i].m_color[j] = glm::linearRand(0.4f, 1.0f);
			// TODO: ADD CONFIG FOR FUTURE ALPHA CHANNEL
		}
	}
}

void NormalizeXYZ(glm::vec3* p)
{
	float length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
	p->x /= length;
	p->y /= length;
	p->z /= length;
}

void ShapeGenerator::BuildSphere()
{
	GenerateSphere(sphereFacets, NUMSPHEREITERATIONS);
	for (GLuint j = 0; j < NUMSPHEREFACETS; ++j)
	{
		sphereVerts[3 * j + 0] = sphereFacets[j].p1;

		sphereVerts[3 * j + 1] = sphereFacets[j].p2;

		sphereVerts[3 * j + 2] = sphereFacets[j].p3;

		sphereIndices[6 * j + 0] = 3 * j + 0;
		sphereIndices[6 * j + 1] = 3 * j + 1;
		sphereIndices[6 * j + 2] = 3 * j + 1;
		sphereIndices[6 * j + 3] = 3 * j + 2;
		sphereIndices[6 * j + 4] = 3 * j + 2;
		sphereIndices[6 * j + 5] = 3 * j + 0;
	}
}

int ShapeGenerator::GenerateSphere(Facet3 * facets, int iterations)
{
	glm::vec3 p[6] =
	{
		glm::vec3(+0, +0, +1),
		glm::vec3(+0, +0, -1),
		glm::vec3(-1, -1, +0),
		glm::vec3(+1, -1, +0),
		glm::vec3(+1, +1, +0),
		glm::vec3(-1, +1, +0)
	};
	glm::vec3 pa, pb, pc;
	int numFacets = 0;
	int ntold;

	/* Create the level 0 object */
	float a = (float)(1 / sqrt(2.0));
	for (int i = 0; i < 6; i++)
	{
		p[i].x *= a;
		p[i].y *= a;
	}

	facets[0].p1 = p[0];
	facets[0].p2 = p[3];
	facets[0].p3 = p[4];
	facets[1].p1 = p[0];
	facets[1].p2 = p[4];
	facets[1].p3 = p[5];
	facets[2].p1 = p[0];
	facets[2].p2 = p[5];
	facets[2].p3 = p[2];
	facets[3].p1 = p[0];
	facets[3].p2 = p[2];
	facets[3].p3 = p[3];
	facets[4].p1 = p[1];
	facets[4].p2 = p[4];
	facets[4].p3 = p[3];
	facets[5].p1 = p[1];
	facets[5].p2 = p[5];
	facets[5].p3 = p[4];
	facets[6].p1 = p[1];
	facets[6].p2 = p[2];
	facets[6].p3 = p[5];
	facets[7].p1 = p[1];
	facets[7].p2 = p[3];
	facets[7].p3 = p[2];
	numFacets = 8;
	if (iterations < 1) return numFacets;

	/* Bisect each edge and move to the surface of a unit sphere */
	for (int it = 0; it < iterations; it++)
	{
		ntold = numFacets;
		for (int i = 0; i < ntold; i++)
		{
			pa.x = (facets[i].p1.x + facets[i].p2.x) / 2;
			pa.y = (facets[i].p1.y + facets[i].p2.y) / 2;
			pa.z = (facets[i].p1.z + facets[i].p2.z) / 2;
			pb.x = (facets[i].p2.x + facets[i].p3.x) / 2;
			pb.y = (facets[i].p2.y + facets[i].p3.y) / 2;
			pb.z = (facets[i].p2.z + facets[i].p3.z) / 2;
			pc.x = (facets[i].p3.x + facets[i].p1.x) / 2;
			pc.y = (facets[i].p3.y + facets[i].p1.y) / 2;
			pc.z = (facets[i].p3.z + facets[i].p1.z) / 2;

			NormalizeXYZ(&pa);
			NormalizeXYZ(&pb);
			NormalizeXYZ(&pc);

			facets[numFacets].p1 = facets[i].p1;
			facets[numFacets].p2 = pa;
			facets[numFacets].p3 = pc;

			numFacets++;
			facets[numFacets].p1 = pa;
			facets[numFacets].p2 = facets[i].p2;
			facets[numFacets].p3 = pb;

			numFacets++;
			facets[numFacets].p1 = pb;
			facets[numFacets].p2 = facets[i].p3;
			facets[numFacets].p3 = pc;

			numFacets++;
			facets[i].p1 = pa;
			facets[i].p2 = pb;
			facets[i].p3 = pc;
		}
	}
	return numFacets;
}