#include "GameLogger.h"
#include "ConfigReader.h"

#include "ObjReader.h"



int main(/*int argc, char** argv*/)
{
	if (!GameLogger::Initialize("..\\Data\\Logs\\ObjConverter", "ObjConverter.log")) return 1;

	ConfigReader *cfg = ConfigReader::GetInstance();

	if (!cfg->Initialize("..\\Data\\ObjConverter.config")) return 2;

	const char  modelKey[100] = "ObjConverter.ModelToConvert";
	const char* modelToConvert;
	if (!cfg->GetStringForKey(modelKey, modelToConvert))
	{
		GameLogger::Log(MsgType::cFatalError, "Unable to find Key [%s] for filename.\n", modelKey);
		GameLogger::Log(MsgType::ConsoleOnly, "Exiting Program...\n");
		return -1;
	}

	ObjReader objReader(cfg);
	if (!objReader.Initialize(modelToConvert)) return 3;

	bool analyze;
	if (!cfg->GetBoolForKey("ObjConverter.Mode.Analyze", analyze)) return 4;

	if (analyze)
	{
		if (!objReader.AnalyzeFile()) return 5;
	}
	else
	{
		if (!objReader.ProcessFile()) return 6;
		if (!objReader.BuildSceneFile()) return 7;
	}

	objReader.Shutdown();

	cfg->Shutdown();

	GameLogger::Shutdown();

	return 0;
}