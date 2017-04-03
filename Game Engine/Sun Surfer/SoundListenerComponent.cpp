#include "SoundListenerComponent.h"

#include "CollisionComponent.h"

bool SoundListenerComponent::Update(Entity & entity, float /*m_dt*/)
{
	CollisionComponent* collision = GetSiblingComponent<CollisionComponent>();
	YSE::Listener().setPosition(YSE::Vec(entity.worldSpace.position.x, collision ? entity.worldSpace.position.y + collision->GetHeight() : entity.worldSpace.position.y, -entity.worldSpace.position.z));
	YSE::Listener().setOrientation(YSE::Vec(entity.worldSpace.lookDirection.x, entity.worldSpace.lookDirection.y, -entity.worldSpace.lookDirection.z));
	return true;
}