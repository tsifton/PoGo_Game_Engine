#ifndef SHAPEMANAGER_H_
#define SHAPEMANAGER_H_

#include "GL\glew.h"

#include "ConfigReader.h"
#include "BinaryWriter.h"
#include "GraphicsObject.h"
#include "CollidableObject.h"
#include "Light.h"

class ShapeManager
{

public:
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

};
#endif // !SHAPEMANAGER_H_
