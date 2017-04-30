#include "GraphicalComponent.h"

#include <cstring>
#include "RenderEngine.h"

bool GraphicalComponent::Initialize()
{
	/*RenderEngine::LoadMeshFromSceneFile(m_sceneFilepath, &m_gob, m_shaderID);
	RenderEngine::AddGraphicalObject(&m_gob);
	m_gob.material.AddTexture(TextureLoader::GetTexture(m_textureFilepath));*/
	return true;
}

bool GraphicalComponent::Update(Entity & entity, float /*m_dt*/)
{
	BuildModelToWorldMatrix(entity.worldSpace.position, entity.worldSpace.yawMatrix, entity.worldSpace.scale);
	return true;
}

void GraphicalComponent::SetSceneFilepath(const char * filepath)
{
	if (filepath != nullptr) strncpy_s(m_sceneFilepath, filepath, 256);
}

void GraphicalComponent::SetTextureFilepath(const char * filepath)
{
	if (filepath != nullptr) strncpy_s(m_textureFilepath, filepath, 256);
}

void GraphicalComponent::SetShader(GLuint shaderID)
{
	m_shaderID = shaderID;
}

void GraphicalComponent::SetUniformCallback(std::function<void(GraphicsObject*gob, ShaderProgram*shader)> callback)
{
	m_gob.SetUniformCallback(callback);
}

void GraphicalComponent::BuildModelToWorldMatrix(glm::vec3 position, glm::mat4 rotate, glm::vec3 scale)
{
	/*m_gob.BuildModelToWorldMatrix(position, rotate, scale);*/
}