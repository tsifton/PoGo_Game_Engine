#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

// Trevor Sifton
// 5/3/16
// Timestamp.h
// Simple Logging for games

#include <ostream>

class Timestamp
{
public:
	static const char* GetStamp(char* const buffer, int bufSize, bool useColons);
};

std::ostream& operator<<(std::ostream& os, Timestamp& stamp);

#endif // ndef TIMESTAMP_H_

