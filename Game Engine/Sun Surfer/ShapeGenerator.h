#ifndef SHAPEGENERATOR_H_
#define SHAPEGENERATOR_H_

#include "GL\glew.h"
#include "Mesh.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include "vec3.hpp"
#pragma warning(pop)

typedef struct { glm::vec3 p1, p2, p3; } Facet3; // 3 points per Facet

struct ColorVertex;
class GraphicsObject;
class ShapeGenerator
{
public:
	static bool Initialize();
	static bool Shutdown();

	static void MakeCube		(GraphicsObject* gob);
	static void MakeDebugCube	(GraphicsObject* gob, glm::vec3 color);
	static void MakePyramid		(GraphicsObject* gob);
	static void MakeDebugArrow	(GraphicsObject* gob);
	static void MakeDebugSphere	(GraphicsObject* gob, glm::vec3 color);
	static void MakeGrid		(GraphicsObject* gob, int size, glm::vec3 color);
	static void MakePlane		(GraphicsObject* gob, GLuint shaderID, glm::vec2 upperLeft, glm::vec2 lowerRight, float k, glm::vec3 color);
	static void MakePoints		(GraphicsObject* gob, GLuint shaderID, int numPoints, glm::vec3 upperLeftBound, glm::vec3 lowerRightBound);

private:
	static void  GenerateGrid		(int size);
	static void  GeneratePlane		(glm::vec2 upperLeft, glm::vec2 lowerRight, float k);
	static Mesh* GenerateMesh		(Mesh* mesh, GLuint shaderID, VertexFormat vertFormat, GLenum mode, IndexDataType indexDataType, void* verts, int numVerts);
	static Mesh* GenerateMesh		(Mesh* mesh, GLuint shaderID, VertexFormat vertFormat, GLenum mode, IndexDataType indexDataType, void* verts, int numVerts, GLuint* indices, int numIndices);
	static void  SetMeshValues		(Mesh* mesh, GLuint shaderID, VertexFormat vertFormat, GLenum mode, IndexDataType indexDataType, void* verts, int numVerts, GLuint* indices, int numIndices);
	static void  SetRandomColors	(ColorVertex* color0, int numVerts);
	static int   GenerateSphere		(Facet3* facets, int iterations);
	static void  BuildSphere		();


private:
	static Mesh m_cubeMesh;
	static Mesh m_debugCubeMesh;
	static Mesh m_pyramidMesh;
	static Mesh m_debugArrowMesh;
	static Mesh m_debugSphereMesh;
	static Mesh m_gridMesh;
	static Mesh m_planeMesh;
	static Mesh m_billboardMesh;

	static GraphicsObject* m_points;
};

#endif // !SHAPEGENERATOR_H_