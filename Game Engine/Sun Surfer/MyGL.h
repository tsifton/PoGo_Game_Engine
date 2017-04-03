#ifndef MYGL_H_
#define MYGL_H_

// Trevor Sifton
// 6/21/2016
// MyGL.h
// Utility class for OpenGL

#include <GL\glew.h>
#include "MsgType.h"

class MyGL
    {
    public:
        static const char* GetErrorMsg(GLenum errNum);
        static bool        TestForError(MsgType msgType, const char* msg);

        template<typename ...Args>
        static bool TestForError (MsgType msgType, const char* const format, Args ...args);
    };

template<typename ...Args>
inline bool MyGL::TestForError (MsgType msgType, const char* const format, Args ...args)
    {
    const int bufSize = 400;
    char      buffer[bufSize];
    sprintf_s (buffer, bufSize, format, args...);
    return TestForError (msgType, buffer);
    }

#endif // ndef MYGL_H_