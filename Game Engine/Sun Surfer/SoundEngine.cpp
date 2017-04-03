#include "SoundEngine.h"

#include "GameLogger.h"

#include "GL\glew.h"
#include "CollisionDetection.h"

ConfigReader* SoundEngine::m_cfg;

bool SoundEngine::Initialize(ConfigReader* cfg)
{
	if (!YSE::System().init())
	{
		GameLogger::Log(MsgType::cFatalError, "SoundEngine::Initialize() - Unable to succesfully initialize the Sound Engine.\n");
		return false;
	}
	m_cfg = cfg;
	YSE::System().occlusionCallback(occlusionCallback);
	return true;
}

bool SoundEngine::Shutdown()
{
	GameLogger::Log(MsgType::Info, "SoundEngine::Shutdown() - Successfully shutdown the Sound Engine.\n");
	YSE::System().close();
	return true;
}

void SoundEngine::Update()
{
	YSE::System().update();
}

float SoundEngine::occlusionCallback(const YSE::Vec & source, const YSE::Vec & listener)
{
	RaycastHit hit;
	glm::vec3 rayOrigin = glm::vec3(listener.x, listener.y, -listener.z);
	glm::vec3 rayTarget = glm::vec3(source.x, source.y, -source.z);
	glm::vec3 ray = rayTarget - rayOrigin;
	glm::vec3 rayDirection = glm::normalize(ray);
	float maxDistance = glm::distance(rayTarget, rayOrigin);
	if (CollisionDetection::RayCast(rayOrigin, rayDirection, hit, maxDistance))
	{
		return 0.80f;
	}
	return 0.0f;
}
