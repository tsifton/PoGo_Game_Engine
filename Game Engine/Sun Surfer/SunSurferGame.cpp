#include "SunSurferGame.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#pragma warning(disable: 4127)
#pragma warning(disable: 4251)
#include <QtGui\qmouseevent>
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

#include "GameLogger.h"
#include "ConfigReader.h"
#include "RenderEngine.h"
#include "SoundEngine.h"
#include "MyWindow.h"
#include "MyGL.h"
#include "MyKeyboard.h"

bool SunSurferGame::Initialize()
{
	if (!InitializeGL()) return false;
	if (!InitializeKeys()) return false;
	if (!RenderEngine::Initialize()) return false;
	/*if (!SoundEngine::Initialize(m_cfg)) return false;*/
	/*m_entityManager.BuildDebugCamera("Camera");*/
	m_entityManager.BuildPlayer("Player");
	/*m_entityManager.BuildStereo("Stereo");
	m_entityManager.BuildFirepit("Firepit");*/
	if (!m_shapeManager.Initialize()) return false;
	GetConfigValues();
	m_cfg->RegisterForConfigChanges(std::bind(&SunSurferGame::GetConfigValues, this));

	GameLogger::Log(MsgType::Process, "SunSurferGame::Initialize() successful.\n");
	return true;
}

bool SunSurferGame::InitializeGL()
{
	glViewport(0, 0, m_window->width(), m_window->height());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	return (!MyGL::TestForError(MsgType::cFatalError, "SunSurferGame::InitializeGL() - Unable to successfully enable depth test or culling.\n"));
}

bool SunSurferGame::InitializeKeys()
{
	if (!m_keyboard->AddToggle('P', &m_paused, true)) return false;
	if (!m_keyboard->AddKeys("C90E12345678")) return false;
	return true;
}

bool SunSurferGame::Shutdown()
{
	if (!m_shapeManager.Shutdown()) return false;
	if (!SoundEngine::Shutdown()) return false;
	if (!RenderEngine::Shutdown()) return false;
	if (!m_window->Shutdown()) return false;
	GameLogger::Log(MsgType::Process, "SunSurferGame::Shutdown() successful.\n");
	return true;
}

void SunSurferGame::Update(float dt)
{
	m_keyboard->Update(dt);
	ProcessInput(dt);
	m_entityManager.Update(dt);
	if (!m_paused)
	{
		SoundEngine::Update();
		m_shapeManager.Update(dt);
	}
	ShowFrameRate(dt);
}

void SunSurferGame::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 worldToViewMat = m_entityManager.GetWorldToViewMatrix();
	glm::mat4 projectionMat = m_perspective.GetPerspective();
	RenderEngine::Draw(worldToViewMat, projectionMat);
}

void SunSurferGame::ProcessInput(float /*dt*/)
{
	if (m_keyboard->KeyWasPressed('0')) m_cfg->ProcessConfig();
	if (m_keyboard->KeyIsDown('X') || m_keyboard->KeyIsDown(VK_ESCAPE)) Shutdown();

	/*if (m_keyboard->KeyWasPressed('9') || m_keyboard->KeyWasPressed('E'))
	{
		if (m_camera.ObjectIsAttached()) m_camera.UnattachObject();
		else m_camera.AttachObject(RenderEngine::GetNearestObjectToPosition(m_camera.GetPosition()));
	}*/
}

void SunSurferGame::MouseMove(QMouseEvent * e)
{
	static int oldX = 0;
	static int oldY = 0;
	if (e->buttons() & Qt::RightButton)
	{
		m_window->setCursor(Qt::BlankCursor);
		int deltaX = e->x() - oldX;
		int deltaY = e->y() - oldY;
		m_entityManager.GetMouse()->MoveMouse(deltaX, deltaY);
	}
	else
	{
		m_window->setCursor(Qt::ArrowCursor);
	}
	oldX = e->x();
	oldY = e->y();
}

void SunSurferGame::MouseScroll(QWheelEvent * e)
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;
	m_entityManager.GetMouse()->ScrollMouse(numSteps);
}

void SunSurferGame::OnResizeWindow()
{
	if (!m_window) return;
	float aspect = m_window->GetAspectRatio();
	m_perspective.SetAspect(aspect);
	glViewport(0, 0, m_window->width(), m_window->height());
}

void SunSurferGame::GetConfigValues()
{
	m_cfg->GetFloatForKey("SunSurfer.VerticalFoV", m_vertFoV);
	m_cfg->GetFloatForKey("SunSurfer.NearClipPlane", m_zNear);
	m_cfg->GetFloatForKey("SunSurfer.FarClipPlane", m_zFar);

	m_perspective.SetPerspective(m_vertFoV, m_window->GetAspectRatio(), m_zNear, m_zFar);

	m_cfg->GetFloatForKey("SunSurfer.FpsInterval", m_fpsInterval);
	if (m_fpsInterval < 0.5f) m_fpsInterval = 0.5f;
}

void SunSurferGame::ShowFrameRate(float dt)
{
	static unsigned numFrames = 0;
	static float    timeAccumulator = 0.0f;

	++numFrames;
	timeAccumulator += dt;

	if (timeAccumulator >= m_fpsInterval)
	{
		printf("FPS: %6.1f over %3.1f seconds.\n", numFrames / timeAccumulator, m_fpsInterval);
		timeAccumulator = 0;
		numFrames = 0;
	}
}