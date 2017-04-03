#include "GameLogger.h"
#include <iostream>
#include "Timestamp.h"


std::ofstream GameLogger::m_logStream;
int GameLogger::m_counts[int(MsgType::NumTypes)]{};

bool GameLogger::Initialize(const char * const path, const char * const filename)
{
	const int pathLimit = 260;
	char filepath[pathLimit] = "";

	const int stampSize = 80;
	char stamp[stampSize];
	Timestamp::GetStamp(stamp, stampSize, false);

	Filepath::Create(filepath, pathLimit, path, stamp, filename);

	m_logStream.open(filepath, std::ofstream::out | std::ofstream::trunc);
	return IsFileOpen(filepath);
}

bool GameLogger::Shutdown(bool showStats)
{
	Log(MsgType::Process, "GameLogger::Shutdown() successful.\n");
	m_logStream.close();
	if (showStats) ShowStats(std::cout);
	return true;
}

void GameLogger::ShowStats(std::ostream& os)
{
	os << "\nBegin Log Message Count by Type\n";
	for (int i = 0; i < int(MsgType::NumTypes); i++)
	{
		int count = GameLogger::m_counts[i];
		if (count > 0)
		{
			os << "   " << (MsgType)i << "= " << count << std::endl;
		}
	}
	os << "End Log Message Count by Type\n\n";
}

void GameLogger::WriteLog(MsgType type, const char* const msg)
{
	++m_counts[int(type)];
	char fileOutput[bufSize] = "";
	if (ConsoleOut(type)) std::cout << type << fileOutput << msg;
	if (type != MsgType::ConsoleOnly)
	{
		Timestamp time;
		m_logStream << time << " " << type << fileOutput << msg;
	}
}

bool GameLogger::IsFileOpen(const char* const filepath)
{
	if (!m_logStream)
	{
		std::cout << "Unable to open file [" << filepath << "].\n";
		return false;
	}
	GameLogger::Log(MsgType::Info, "GameLogger::Initialize() opened log file [%s] for writing.\n", filepath);
	GameLogger::Log(MsgType::Process, "GameLogger::Initialize() successful.\n");
	return true;
}
