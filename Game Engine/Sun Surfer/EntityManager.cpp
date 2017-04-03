#include "EntityManager.h"

#include "GameLogger.h"
#include "RenderEngine.h"
#include "UniformCallback.h"
#include "SceneLoader.h"

#include "Entity.h"
#include "MovementComponent.h"
#include "KeyboardComponent.h"
#include "GraphicalComponent.h"
#include "CollisionComponent.h"
#include "GravityComponent.h"
#include "PhysicsComponent.h"
#include "JumpComponent.h"
#include "SoundEmitterComponent.h"
#include "SoundListenerComponent.h"

#include "PhasorComponent.h"
#include "ProjectileMovementComponent.h"
#include "ProjectilePhysicsComponent.h"

Entity debugCamera;
KeyboardComponent debugCameraKeyboardComponent;
MovementComponent debugCameraMovementComponent;

Entity player;
MovementComponent		playerMovementComponent;
KeyboardComponent		playerKeyboardComponent;
GraphicalComponent		playerGraphicalComponent;
CollisionComponent		playerCollisionComponent;
JumpComponent			playerJumpComponent;
GravityComponent		playerGravityComponent;
PhysicsComponent		playerPhysicsComponent;
PhasorComponent			playerPhasorComponent;
SoundListenerComponent	playerListenerComponent;
SoundEmitterComponent	playerEmitterComponent;

Entity bullet;
GraphicalComponent			bulletGraphicalComponent;
CollisionComponent			bulletCollisionComponent;
GravityComponent			bulletGravityComponent;
ProjectilePhysicsComponent	bulletPhysicsComponent;
ProjectileMovementComponent	bulletMovementComponent;

Entity dargon;
GraphicalComponent		dargonGraphicalComponent;
SoundEmitterComponent	dargonEmitterComponent;

Entity stereo;
GraphicalComponent		stereoGraphicalComponent;
SoundListenerComponent	stereoListenerComponent;
SoundEmitterComponent	stereoEmitterComponent;

Entity firepit;
GraphicalComponent		firepitGraphicalComponent;
SoundListenerComponent	firepitListenerComponent;
SoundEmitterComponent	firepitEmitterComponent;

CameraComponent		cameraComponent;
MouseComponent		mouseComponent;

bool EntityManager::Update(float dt)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		Entity* entity = m_entities[i];
		if (entity != nullptr)
		{
			entity->Update(dt);
		}
	}
	return true;
}

glm::mat4 EntityManager::GetWorldToViewMatrix()
{
	return cameraComponent.GetWorldToViewMatrix();
}

MouseComponent* EntityManager::GetMouse()
{
	return &mouseComponent;
}

CameraComponent * EntityManager::GetCamera()
{
	return &cameraComponent;
}

bool EntityManager::AddEntity(Entity * entity)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (m_entities[i] == nullptr)
		{
			m_entities[i] = entity;
			return true;
		}
	}

	GameLogger::Log(MsgType::cFatalError, "EntityManager::AddEntity() - Entity Manager has no room left for entities, size of entity buffer = %d.\n", MAX_ENTITIES);
	return false;
}

Entity * EntityManager::BuildDebugCamera(const char * name)
{
	debugCamera.SetName(name);
	debugCamera.AddComponent(&mouseComponent, "Mouse");
	debugCamera.AddComponent(&playerKeyboardComponent, "Keyboard");
	debugCamera.AddComponent(&playerMovementComponent, "Movement");
	debugCamera.AddComponent(&cameraComponent, "Camera");

	debugCamera.worldSpace.position = glm::vec3(0.0f, 15.0f, 0.0f);
	debugCamera.worldSpace.scale = glm::vec3(0.6f);

	debugCamera.modelSpace.lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	debugCamera.modelSpace.rightDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
	debugCamera.modelSpace.upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	playerMovementComponent.SetForwardSpeed(40.0f);
	playerMovementComponent.SetStrafeSpeed(40.0f);
	playerMovementComponent.SetYawSpeed(-.65f);
	playerMovementComponent.SetPitchSpeed(-.65f);

	cameraComponent.SetCameraOffset(glm::vec3(0.0f, 0.0f, 0.0f));
	cameraComponent.SetLookAtPointOffset(glm::vec3(0.0f, 0.0f, 0.0f));

	debugCamera.Intialize();
	AddEntity(&debugCamera);
	return &debugCamera;
}

Entity * EntityManager::BuildPlayer(const char * name)
{
	player.SetName(name);
	player.AddComponent(&mouseComponent,			"Mouse");
	player.AddComponent(&playerKeyboardComponent,	"Keyboard");
	player.AddComponent(&playerMovementComponent,	"Movement");
	player.AddComponent(&playerJumpComponent,		"Jump");
	/*player.AddComponent(&playerGravityComponent,	"Gravity");*/
	player.AddComponent(&playerPhysicsComponent,	"Physics");
	player.AddComponent(&playerCollisionComponent,	"Collision");
	player.AddComponent(&playerGraphicalComponent,	"Graphics");
	player.AddComponent(&cameraComponent,			"Camera");
	player.AddComponent(&playerPhasorComponent,		"Phasor");
	player.AddComponent(&playerEmitterComponent,	"SoundEmitter");
	player.AddComponent(&playerListenerComponent,	"SoundListener");

	playerEmitterComponent.SetVolume(0.7f);
	playerEmitterComponent.SetChannel(&YSE::ChannelFX());
	playerEmitterComponent.SetOnLoop(true);
	playerEmitterComponent.SetIs3D(false);
	playerEmitterComponent.SetFilePath("..\\Data\\Sounds\\Footsteps.wav");
	playerEmitterComponent.SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));

	player.worldSpace.position = glm::vec3(0.0f, 10.0f, 0.0f);
	player.worldSpace.scale = glm::vec3(0.4);

	player.modelSpace.lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	player.modelSpace.rightDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
	player.modelSpace.upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	playerMovementComponent.SetForwardSpeed(15.0f);
	playerMovementComponent.SetStrafeSpeed(10.0f);
	playerMovementComponent.SetYawSpeed(-.65f);
	playerMovementComponent.SetPitchSpeed(-.65f);

	playerJumpComponent.SetJumpSpeed(20.0f);

	playerCollisionComponent.SetSceneFilepath("..\\Data\\Scenes\\Character.PCT.scene");

	playerGravityComponent.SetAcceleration(-45.0f);
	
	playerGraphicalComponent.SetShader(Shader::TextureLighting);
	playerGraphicalComponent.SetSceneFilepath("..\\Data\\Scenes\\Character.PTN.scene");
	playerGraphicalComponent.SetTextureFilepath("..\\Data\\textures\\skin_zombie.bmp");
	playerGraphicalComponent.SetUniformCallback(UniformCallback::TextureLighting);

	playerPhasorComponent.AddEntityBuilder(this);

	cameraComponent.SetCameraOffset(glm::vec3(0.0f, 3.0f, -6.0f));
	cameraComponent.SetLookAtPointOffset(glm::vec3(0.0f, 5.5f, 0.0f));
	
	player.Intialize();
	AddEntity(&player);
	return &player;
}

Entity * EntityManager::BuildBullet(const char * name, glm::vec3 position, glm::vec3 /*velocity*/)
{
	bullet.SetName(name);
	bullet.AddComponent(&bulletMovementComponent,	"Movement");
	bullet.AddComponent(&bulletGravityComponent,	"Gravity");
	bullet.AddComponent(&bulletPhysicsComponent,	"Physics");
	bullet.AddComponent(&bulletCollisionComponent,	"Collision");
	bullet.AddComponent(&bulletGraphicalComponent,	"Graphics");

	bullet.worldSpace.position = position;
	bullet.worldSpace.scale = glm::vec3(0.15f);

	bullet.modelSpace.lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	bullet.modelSpace.rightDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
	bullet.modelSpace.upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	bulletCollisionComponent.SetSceneFilepath("..\\Data\\Scenes\\Character.PCT.scene");

	bulletGravityComponent.SetAcceleration(-30.0f);

	bulletGraphicalComponent.SetShader(Shader::TextureLighting);
	bulletGraphicalComponent.SetSceneFilepath("..\\Data\\Scenes\\Character.PTN.scene");
	bulletGraphicalComponent.SetTextureFilepath("..\\Data\\textures\\skin_zombie.bmp");
	bulletGraphicalComponent.SetUniformCallback(UniformCallback::LitCharacter);

	bullet.Intialize();
	AddEntity(&bullet);
	return &bullet;
}

Entity * EntityManager::BuildDargon(const char * name)
{
	dargon.SetName(name);
	dargon.AddComponent(&dargonGraphicalComponent,	"Graphics");
	dargon.AddComponent(&dargonEmitterComponent,	"SoundEmitter");

	dargon.worldSpace.position = glm::vec3(0.0f, 5.0f, 100.0f);
	dargon.worldSpace.scale = glm::vec3(3.0f);

	dargon.modelSpace.lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	dargon.modelSpace.upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	dargon.modelSpace.rightDirection = glm::vec3(1.0f, 0.0f, 0.0f);

	dargonGraphicalComponent.SetShader(Shader::Texture);
	dargonGraphicalComponent.SetSceneFilepath("..\\Data\\Scenes\\Dargon.PCT.scene");
	dargonGraphicalComponent.SetTextureFilepath("..\\Data\\textures\\skin_dargon.bmp");
	/*dargonGraphicalComponent.SetUniformCallback(UniformCallback::LitDargon);*/

	dargonEmitterComponent.SetFilePath("..\\Data\\Sounds\\Hello My Baby.ogg");
	dargonEmitterComponent.SetChannel(&YSE::ChannelMusic());
	dargonEmitterComponent.SetIs3D();
	dargonEmitterComponent.SetOnLoop(true);
	dargonEmitterComponent.SetVolume(0.5f);
	dargonEmitterComponent.SetRange(40.0f);
	dargonEmitterComponent.EnableOcclusion();

	dargon.Intialize();
	AddEntity(&dargon);
	return &dargon;
}

Entity * EntityManager::BuildStereo(const char * name)
{
	stereo.SetName(name);
	stereo.AddComponent(&stereoGraphicalComponent,	"Graphics");
	stereo.AddComponent(&stereoEmitterComponent,	"SoundEmitter");

	stereoEmitterComponent.SetFilePath("..\\Data\\Sounds\\LOTR.ogg");
	stereoEmitterComponent.SetChannel(&YSE::ChannelMusic());
	stereoEmitterComponent.SetIs3D();
	stereoEmitterComponent.SetOnLoop(true);
	stereoEmitterComponent.SetVolume(0.8f);
	stereoEmitterComponent.SetRange(60.0f);
	stereoEmitterComponent.EnableOcclusion();
	stereoEmitterComponent.SetPosition(glm::vec3(0.0f, 1.0f, -3.0f));

	stereo.worldSpace.position = glm::vec3(41.0f, 14.0f, -84.0f);
	stereo.worldSpace.scale = glm::vec3(1.0f);

	stereoGraphicalComponent.SetShader(Shader::PassThrough);
	stereoGraphicalComponent.SetSceneFilepath("..\\Data\\Scenes\\Stereo.PN.scene");
	/*stereoGraphicalComponent.SetTextureFilepath("..\\Data\\textures\\skin_zombie.bmp");*/
	stereoGraphicalComponent.SetUniformCallback(UniformCallback::LitCharacter);

	stereo.Intialize();
	AddEntity(&stereo);
	return &stereo;
}

Entity * EntityManager::BuildFirepit(const char * name)
{
	firepit.SetName(name);
	firepit.AddComponent(&firepitGraphicalComponent,	"Graphics");
	firepit.AddComponent(&firepitEmitterComponent,		"SoundEmitter");

	firepitEmitterComponent.SetFilePath("..\\Data\\Sounds\\Fire.wav");
	firepitEmitterComponent.SetChannel(&YSE::ChannelMusic());
	firepitEmitterComponent.SetIs3D();
	firepitEmitterComponent.SetOnLoop(true);
	firepitEmitterComponent.SetVolume(0.4f);
	firepitEmitterComponent.SetRange(35.0f);
	firepitEmitterComponent.EnableOcclusion();
	firepitEmitterComponent.SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

	firepit.worldSpace.position = glm::vec3(-42.0f, 0.0f, 0.0f);
	firepit.worldSpace.scale = glm::vec3(1.0f);

	firepitGraphicalComponent.SetShader(Shader::TextureLighting);
	firepitGraphicalComponent.SetSceneFilepath("..\\Data\\Scenes\\Firepit.PTN.scene");
	firepitGraphicalComponent.SetTextureFilepath("..\\Data\\textures\\firepit_texture.bmp");
	firepitGraphicalComponent.SetUniformCallback(UniformCallback::LitCharacter);

	firepit.Intialize();
	AddEntity(&firepit);
	return &firepit;
}
