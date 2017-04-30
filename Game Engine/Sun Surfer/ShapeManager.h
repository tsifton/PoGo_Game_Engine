#ifndef SHAPEMANAGER_H_
#define SHAPEMANAGER_H_

#include "GL\glew.h"

#include "ConfigReader.h"
#include "BinaryWriter.h"
#include "GraphicsObject.h"
#include "CollidableObject.h"
#include "Light.h"
#include "RenderEngine.h"

class ShapeManager
{
public:
	ShapeManager(RenderEngine& engine) : m_renderEngine(engine) {}

	bool Initialize();
	bool Shutdown();

	void Update(float dt);

private:
	bool InitializeShapes();
	void SetSettingsForLightingTestObjects();
	void RotateLightAroundObject(Light& light, GraphicsObject& object, float dt);
	void RotateObject(GraphicsObject& gob, float dt);

private:
	GraphicsObject m_debugArrow;
	GraphicsObject m_grid;

	GraphicsObject m_dargon;
	GraphicsObject m_hideout;
	GraphicsObject m_torus;

	CollidableObject m_hideoutCollision;

	static const int NUM_LIGHTS = 1;
	Light m_lightingCubes[NUM_LIGHTS];

	static ConfigReader* m_cfg;
	BinaryWriter m_writer;

	RenderEngine& m_renderEngine;

};
#endif // !SHAPEMANAGER_H_
