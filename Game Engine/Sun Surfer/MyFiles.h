#ifndef MYFILES_H_
#define MYFILES_H_

#include <fstream>

class MyFiles
{
public:
	static char* ReadFileIntoString(const char* const filename);

private:
	static std::ifstream m_ifs;
};

#endif // !MYFILES_H_