#include "MyFiles.h"
#include "GameLogger.h"
#include "Filepath.h"


std::ifstream MyFiles::m_ifs;

char * MyFiles::ReadFileIntoString(const char * const filename)
{
	const int pathSize = 100;
	char filepath[pathSize];
	Filepath::Create(filepath, pathSize, "..\\Data\\Shaders\\", "", filename);

	m_ifs.open(filepath);
	if (!m_ifs.good())
	{
		GameLogger::Log(MsgType::cFatalError, "MyFiles::ReadFileIntoString() was unable to open file at [%s].\n", filepath);
		return nullptr;
	}

	// get pointer to associated buffer object
	std::filebuf* pbuf = m_ifs.rdbuf();
	// get size of file
	std::streampos size = pbuf->pubseekoff(0, m_ifs.end);
	long buflen = 1 + static_cast<long> (size);

	GameLogger::Log(MsgType::Info, "MyFiles::ReadFileIntoString() Created a buffer of size [%d] to hold the contents of the file [%s].\n", buflen, filepath);

	if (buflen <= 0) 
	{
		GameLogger::Log(MsgType::cFatalError, "MyFiles::ReadFileIntoString() was unable to seek to the end of the file [%s].\n", filepath);
		return nullptr;
	}

	const int bufSize = 10000;
	char* buffer = new char[bufSize]{ 0 };

	pbuf->pubseekpos(0, m_ifs.in);
	pbuf->sgetn(buffer, bufSize);

	m_ifs.close();

	GameLogger::Log(MsgType::Info, "\n\n	--------- File [%s] ---------\n%s\n	-------- End of [%s] --------\n\n", filepath, buffer, filepath);

	return buffer;
}
