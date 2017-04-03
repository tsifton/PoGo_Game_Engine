#include "MsgType.h"



char* LogMsgWords[] =
{
	"[UnknownMsgType  ] ",
	"[ConsoleOnly     ] ",
	"[Process         ] ",
	"[Process         ] ",
	"[Info            ] ",
	"[Info            ] ",
	"[Debug           ] ",
	"[Debug           ] ",
	"[Warning         ] ",
	"[Warning         ] ",
	"[Error           ] ",
	"[Error           ] ",
	"[Fatal Error     ] ",
	"[Fatal Error     ] ",
	"[Programmer Error] ",
	"[Programmer Error] "
};	  

std::ostream & operator<<(std::ostream & os, MsgType msgtype)
{
	return os << LogMsgWords[GetMsgType(msgtype)];
}

bool ConsoleOut(MsgType msgtype)
{
	return GetMsgType(msgtype) & 1;
}

int GetMsgType(MsgType msgtype)
{
	return int(msgtype);
}
