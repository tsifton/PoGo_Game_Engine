#ifndef FILEPATH_H_
#define FILEPATH_H_

// Trevor Sifton
// 5/3/16
// Filepath.h
// Simple Logging for games

class Filepath
{
public:
	static char* Create(char* const buffer, int bufSize, const char* const path, const char* const stamp, const char* const filename);
};

#endif // ndef FILEPATH_H_

