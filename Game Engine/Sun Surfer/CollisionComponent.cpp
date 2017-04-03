#include "CollisionComponent.h"

#include "CollisionDetection.h"
#include "SceneLoader.h"

bool CollisionComponent::Initialize()
{
	if (SceneLoader::GetMeshFromSceneFile(m_sceneFilepath, m_collidable.m_mesh))
	{
		/*CollisionDetection::AddCollidable(&m_collidable);*/
		return true;
	}
	return false;
}

bool CollisionComponent::Update(Entity & entity, float /*m_dt*/)
{
	BuildModelToWorldMatrix(entity.worldSpace.position, entity.worldSpace.yawMatrix, entity.worldSpace.scale);
	return true;
}

void CollisionComponent::SetSceneFilepath(const char * filepath)
{
	if (filepath != nullptr) strncpy_s(m_sceneFilepath, filepath, 256);
}

void CollisionComponent::BuildModelToWorldMatrix(glm::vec3 position, glm::mat4 rotate, glm::vec3 scale)
{
	m_collidable.BuildModelToWorldMatrix(position, rotate, scale);
}

float CollisionComponent::GetWidth()
{
	return m_collidable.m_mesh->width * m_owner->worldSpace.scale.x;
}

float CollisionComponent::GetHeight()
{
	return m_collidable.m_mesh->height * m_owner->worldSpace.scale.y;
}

float CollisionComponent::GetDepth()
{
	return m_collidable.m_mesh->depth * m_owner->worldSpace.scale.z;
}
