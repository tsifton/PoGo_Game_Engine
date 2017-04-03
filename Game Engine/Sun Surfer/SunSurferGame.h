#ifndef SUNSURFERGAME_H_
#define SUNSURFERGAME_H_

#include "ShaderProgram.h"
#include "ShapeManager.h"
#include "Perspective.h"
#include "Camera.h"
#include "MyKeyboard.h"
#include "EntityManager.h"

class MyWindow;
class QMouseEvent;
class QWheelEvent;
class ConfigReader;
class SunSurferGame
{
public:
	SunSurferGame(ConfigReader* cfg) : m_cfg(cfg) {};

public:
	bool Initialize(MyWindow* window);
	bool InitializeGL();
	bool InitializeKeys();
	bool Shutdown();

	void Update(float dt);
	void Draw();

	void MouseMove(QMouseEvent* e);
	void MouseScroll(QWheelEvent* e);
	void OnResizeWindow();

private:
	void ProcessInput(float dt);
	void ShowFrameRate(float dt);
	void GetConfigValues();

private:
	bool  m_paused = false;
	float m_vertFoV = 60.0f;
	float m_zNear = 0.01f;
	float m_zFar = 100.0f;
	float m_fpsInterval = 1.0f;

	ConfigReader* m_cfg;
	MyWindow* m_window{ nullptr };
	MyKeyboard* m_keyboard = MyKeyboard::GetInstance();

	ShaderProgram m_program;
	ShapeManager m_shapeManager;
	EntityManager m_entityManager;

	Perspective m_perspective;
};

#endif // !SUNSURFERGAME_H_