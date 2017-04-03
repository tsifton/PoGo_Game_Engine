#include "ShapeManager.h"
#include "ShapeGenerator.h"
#include "RenderEngine.h"
#include "UniformCallback.h"
#include "CollisionDetection.h"
#include <sstream>

#pragma warning(push)
#pragma warning(disable :4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

ConfigReader* ShapeManager::m_cfg = ConfigReader::GetInstance();

bool ShapeManager::Initialize()
{
	if (!ShapeGenerator::Initialize()) return false;
	return InitializeShapes();
}

bool ShapeManager::Shutdown()
{
	if (!ShapeGenerator::Shutdown()) return false;
	return true;
}

void ShapeManager::Update(float /*m_dt*/)
{

}

bool ShapeManager::InitializeShapes()
{
	m_cfg->RegisterForConfigChanges(std::bind(&ShapeManager::SetSettingsForLightingTestObjects, this));
	SetSettingsForLightingTestObjects();

	// Generate a DebugArrow
	ShapeGenerator::MakeDebugArrow(&m_debugArrow);
	RenderEngine::AddGraphicalObject(&m_debugArrow);
	m_debugArrow.rotationAxis = glm::vec3(0, 0, 1);
	m_debugArrow.rotateAngle = -90.0f;
	m_debugArrow.BuildModelToWorldMatrix(glm::vec3(0.0f, 5.0f, 0.0f), glm::rotate(glm::mat4(), glm::radians(m_debugArrow.rotateAngle), m_debugArrow.rotationAxis), glm::vec3(1.0f));

	// Generate a Grid
	ShapeGenerator::MakeGrid(&m_grid, 100, glm::vec3(0.0f, 0.0f, 1.0f));
	RenderEngine::AddGraphicalObject(&m_grid);
	m_grid.BuildModelToWorldMatrix(glm::vec3(0.0f, -.2f, 0.0f), glm::mat4(), glm::vec3(1000));
	m_grid.SetUniformCallback(UniformCallback::Debug);

	//// Make a Dargon that has multiple phong
	//RenderEngine::LoadMeshFromSceneFile("..\\Data\\Scenes\\Dargon.PN.scene", &m_dargon, Shader::PhongShadowVolumes);
	//RenderEngine::AddGraphicalObject(&m_dargon);
	//m_dargon.BuildModelToWorldMatrix(glm::vec3(-45.0f, 8.0f, 10.0f), glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(0,1,0)), glm::vec3(7.0f));
	//m_dargon.SetUniformCallback(UniformCallback::MultipleShading);
	//m_dargon.mesh->renderInfo.mode = GL_TRIANGLES;

	//m_dargon.SetMaterialAmbientColor(glm::vec3(1.0f, 0.0f, 0.0f));
	//m_dargon.SetMaterialDiffuseColor(glm::vec3(1.0f, 0.0f, 0.0f));
	//m_dargon.SetMaterialSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//m_dargon.SetMaterialShininessFactor(32);
	//m_dargon.AddLightIndex(0);

	// Make a Hideout that has multiple phong
	RenderEngine::LoadMeshFromSceneFile("..\\Data\\Scenes\\Hideout.PN.scene", &m_hideout, Shader::PhongShadowVolumes);
	/*SceneLoader::GetMeshFromSceneFile("..\\Data\\Scenes\\Hideout.PN.scene", m_hideoutCollision.m_mesh);*/
	/*CollisionDetection::AddCollidable(&m_hideoutCollision);*/
	m_hideoutCollision.BuildModelToWorldMatrix(glm::vec3(-10.0f, 0.0f, -60.0f), glm::mat4(), glm::vec3(1.0f));
	RenderEngine::AddGraphicalObject(&m_hideout);
	m_hideout.BuildModelToWorldMatrix(glm::vec3(-10.0f, 0.0f, -60.0f), glm::mat4(), glm::vec3(1.0f));
	m_hideout.SetUniformCallback(UniformCallback::MultipleShading);

	m_hideout.SetMaterialAmbientColor(glm::vec3(0.5f, 0.5f, 0.5f));
	m_hideout.SetMaterialDiffuseColor(glm::vec3(0.5f, 0.5f, 0.5f));
	m_hideout.SetMaterialSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_hideout.SetMaterialShininessFactor(64);
	m_hideout.AddLightIndex(0);

	// Make a torus that has multiple phong
	RenderEngine::LoadMeshFromSceneFile("..\\Data\\Scenes\\Torus.PN.scene", &m_torus, Shader::PhongShadowVolumes);
	RenderEngine::AddGraphicalObject(&m_torus);
	m_torus.BuildModelToWorldMatrix(glm::vec3(-45.0f, 7.0f, 15.0f), glm::mat4(), glm::vec3(5.0f));
	m_torus.SetUniformCallback(UniformCallback::MultipleShading);
	m_torus.mesh->renderInfo.mode = GL_TRIANGLES;

	m_torus.SetMaterialAmbientColor(glm::vec3(0.0f, 0.0f, 1.0f));
	m_torus.SetMaterialDiffuseColor(glm::vec3(0.0f, 0.0f, 1.0f));
	m_torus.SetMaterialSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_torus.SetMaterialShininessFactor(32);
	m_torus.AddLightIndex(0);

	// Generate the Lighting Cubes
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		RenderEngine::LoadMeshFromSceneFile("..\\Data\\Scenes\\Cube.PN.scene", &m_lightingCubes[i], Shader::MultiplePhong);
		RenderEngine::AddLight(&m_lightingCubes[i]);
		m_lightingCubes[i].isEnabled = true;
		m_lightingCubes[i].SetUniformCallback(UniformCallback::MultipleShading);
		m_lightingCubes[i].rotationSpeed = 10;
		m_lightingCubes[i].rotationAxis = glm::vec3(-1, 0, 0);

		m_lightingCubes[i].SetMaterialEmissiveColor(glm::vec3(1.0f, 1.0f, 1.0f));
		m_lightingCubes[i].SetAmbientColor(glm::vec3(0.2f, 0.2f, 0.2f));
		m_lightingCubes[i].SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		m_lightingCubes[i].SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	return true;
}

void ShapeManager::SetSettingsForLightingTestObjects()
{
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		glm::vec3 lightPosition;
		std::stringstream cfgKey;
		cfgKey << "SunSurfer.LightingTest.Light" << i << ".Position";
		m_cfg->GetFloatsForKey(cfgKey.str().c_str(), 3, &lightPosition[0]);
		m_lightingCubes[i].BuildModelToWorldMatrix(lightPosition, glm::mat4(), glm::vec3(1.0f));
	}
}

void ShapeManager::RotateObject(GraphicsObject& gob, float dt)
{
	gob.rotateAngle += gob.rotationSpeed * dt;
	glm::mat4 rotate = glm::rotate(glm::mat4(), gob.rotateAngle, gob.rotationAxis);
	gob.BuildModelToWorldMatrix(glm::vec3(gob.GetPosition()), rotate, glm::vec3(1.0));
}

void ShapeManager::RotateLightAroundObject(Light& light, GraphicsObject& object, float dt)
{
	glm::vec4 lightPosition = light.GetPosition();
	glm::vec4 objectPosition = object.GetPosition();

	glm::vec4 rotationVec = lightPosition - objectPosition;

	light.rotateAngle = light.rotationSpeed * dt;
	glm::mat4 rotationMat = glm::rotate(glm::mat4(), glm::radians(light.rotateAngle), light.rotationAxis);

	rotationVec = rotationMat * rotationVec;

	light.BuildModelToWorldMatrix(glm::vec3(objectPosition + rotationVec), rotationMat, glm::vec3(1.0f));
}