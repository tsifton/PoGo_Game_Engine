#include "MyGL.h"
#include "GameLogger.h"

const int numEnums = 7;
const char* GLErrEnums[numEnums] =
{
	"GL_NO_ERROR",
	"GL_INVALID_ENUM",
	"GL_INVALID_VALUE",
	"GL_INVALID_OPERATION",
	"GL_STACK_OVERFLOW",
	"GL_STACK_UNDERFLOW",
	"GL_OUT_OF_MEMORY"
};

const char * MyGL::GetErrorMsg(GLenum errEnum)
{
	int errIndex = errEnum % 1279;
	if (errIndex >= 0 || errIndex < numEnums) return GLErrEnums[errIndex];
	else return "Invalid Enum Value";
}

bool MyGL::TestForError(MsgType msgType, const char * msg)
{
	bool errorFound = false;
	for (GLenum errEnum = glGetError(); errEnum != GL_NO_ERROR; errEnum = glGetError())
	{
		errorFound = true;
		GameLogger::Log(msgType, "%s GLErrorMsg:(%s)\n", msg, GetErrorMsg(errEnum));
	}
	return errorFound;
}
