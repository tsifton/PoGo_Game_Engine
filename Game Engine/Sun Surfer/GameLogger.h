#ifndef GAMELOGGER_H_
#define GAMELOGGER_H_

// Trevor Sifton
// 5/3/16
// GameLogger.h
// Simple Logging for games

#include <fstream>
#include "MsgType.h"
#include "Filepath.h"

class GameLogger
{
public: // public static methods
	static bool Initialize(const char* const path, const char* const filename);
	static bool Shutdown(bool showStats = true);

	template<typename...Args>
	static void Log(MsgType type, const char* const format, Args... args);

private: // private static methods
	static void WriteLog(MsgType type, const char* const msg); // pre-pend time, error level, etc
	static bool IsFileOpen(const char* const filepath);
	static void ShowStats(std::ostream& os);

private: // private static data
	static std::ofstream m_logStream;
	static const int bufSize = 20000;
	static int m_counts[int(MsgType::NumTypes)];
};

template<typename ...Args>
inline void GameLogger::Log(MsgType type, const char * const format, Args ...args)
{
	char buffer[bufSize];
	sprintf_s(buffer, bufSize, format, args...);
	WriteLog(type, buffer);
}
#endif // ndef GAMELOGGER_H_