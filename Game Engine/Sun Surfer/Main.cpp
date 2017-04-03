#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4127)
#include "Qt\qapplication.h"
#pragma warning(pop)

#include "SunSurferGame.h"
#include "MyWindow.h"

#include "GameLogger.h"
#include "ConfigReader.h"

bool showFullscreen;
bool showMaximized;
bool showMinimized;

void GetConfigValues(ConfigReader* cfg)
{
	cfg->GetBoolForKey("SunSurfer.ShowFullScreen", showFullscreen);
	cfg->GetBoolForKey("SunSurfer.ShowMaximized", showMaximized);
	cfg->GetBoolForKey("SunSurfer.ShowMinimized", showMinimized);
}

void SetWindow(MyWindow* window)
{
	if (showFullscreen) window->showFullScreen();
	else if (showMaximized) window->showMaximized();
	else if (showMinimized) window->showMinimized();
}

int main(int argc, char** argv)
{
	if (!GameLogger::Initialize("..\\Data\\Logs\\Sun Surfer", "SunSurfer.log")) return 1;

	ConfigReader *cfg = ConfigReader::GetInstance();

	if (!cfg->Initialize("..\\Data\\Sun Surfer.config")) return 2;
	GetConfigValues(cfg);

	QApplication app(argc, argv);
	SunSurferGame game(cfg);
	MyWindow* window = new MyWindow(&app, &game);

	window->show();
	SetWindow(window);

	int appResult = app.exec();

	delete window;

	cfg->Shutdown();

	GameLogger::Shutdown();

	return appResult;
}