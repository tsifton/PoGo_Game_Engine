#ifndef MYWINDOW_H_
#define MYWINDOW_H_

// Trevor Sifton
// 6/21/2016
// MyWindow.h
// class to present window, get mouse events, provide timer

#include "GL\glew.h"

#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4251)
#pragma warning(disable: 4800)
#include "QtOpenGL\qglwidget"
#pragma warning(pop)

#include "Qt\qtimer.h"

class QApplication;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class SunSurferGame;
class MyWindow : public QGLWidget
{
	Q_OBJECT
public:
	MyWindow(QApplication* app, SunSurferGame* game) : m_app(app), m_game(game) {};
	~MyWindow() {};

public:
	bool Initialize();
	bool Shutdown();

	float GetAspectRatio();

protected:
	void initializeGL() { Initialize(); };
	void mouseMoveEvent(QMouseEvent* e) override;
	void resizeEvent(QResizeEvent* e) override;
	void wheelEvent(QWheelEvent* e) override;

private slots:
	void Update();

private:
	QApplication* m_app;
	SunSurferGame* m_game;
	QTimer m_timer;
};

#endif // !MYWINDOW_H_