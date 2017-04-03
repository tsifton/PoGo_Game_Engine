#ifndef MSGTYPE_H_
#define MSGTYPE_H_

// Trevor Sifton
// 5/4/2016
// MsgType.h
// Enum of different Log Message Types

#include <ostream>

enum class MsgType
{
	UnknownLogMsgType = 0,
	ConsoleOnly,
	Process,
	cProcess,
	Info,
	cInfo,
	Debug,
	cDebug,
	Warning,
	cWarning,
	Error,
	cError,
	FatalError,
	cFatalError,
	ProgrammerError,
	cProgrammerError,

	NumTypes // this one is always last
};

std::ostream& operator<<(std::ostream& os, MsgType msgtype);

bool ConsoleOut(MsgType msgtype);    // return true if output to console is indicated
int  GetMsgType(MsgType msgtype);    // returns the msgType, not including the console bit


#endif // ndef MSGTYPE_H_