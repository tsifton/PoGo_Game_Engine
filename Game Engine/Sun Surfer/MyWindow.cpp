#include "SunSurferGame.h"
#include "GameLogger.h"

#pragma warning(push)
#pragma warning(disable: 4127)
#include "Qt\qapplication.h"
#include <QtGui\qmouseevent>
#pragma warning(pop)

#include "MyWindow.h"
#include "GameTime.h"

bool MyWindow::Initialize()
{
	GLenum initResult = glewInit();
	if (initResult != GLEW_OK)
	{
		GameLogger::Log(MsgType::FatalError, "Unable to initialize glew, result was %d.\n", initResult);
		return false;
	}

	if (!GameTime::Initialize() || !m_game->Initialize(this))
	{
		Shutdown();
		return false;
	}

	setMouseTracking(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(Update()));
	m_timer.start();
	
	GameLogger::Log(MsgType::Process, "MyWindow::Initialize() successful.\n");
	return true;
}

bool MyWindow::Shutdown()
{
	m_app->exit();
	GameLogger::Log(MsgType::Process, "MyWindow::Shutdown() successful.\n");
	return true;
}

float MyWindow::GetAspectRatio()
{
	return static_cast<float>(width()) / height();
}

void MyWindow::mouseMoveEvent(QMouseEvent * e)
{
	m_game->MouseMove(e);
}

void MyWindow::resizeEvent(QResizeEvent * /*e*/)
{
	m_game->OnResizeWindow();
}

void MyWindow::wheelEvent(QWheelEvent * e)
{
	m_game->MouseScroll(e);
}

void MyWindow::Update()
{
	float dt = GameTime::GetLastFrameTime();
	m_game->Update(dt); //11%
	m_game->Draw();		//31%
	this->repaint();	//42%
}