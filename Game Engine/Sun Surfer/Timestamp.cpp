#include "Timestamp.h"
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

const char * Timestamp::GetStamp(char * const buffer, int bufSize, bool useColons)
{
	// get date and time
	const char* const format = useColons ? "%F %T." : "%F %H-%M-%S.";
	const char* const milliFormat = useColons ? "%03d" : "%03d.";

	const int milliSize = 5;
	char millibuf[milliSize];
	struct _timeb timebuffer;
	_ftime64_s(&timebuffer);
	unsigned short millitime = timebuffer.millitm;
	sprintf_s(millibuf, milliSize, milliFormat, millitime);

	time_t rawtime;
	time(&rawtime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, bufSize, format, &timeinfo);
	strcat_s(buffer, bufSize, millibuf);

	return buffer;
}

std::ostream& operator<<(std::ostream& os, Timestamp & /*stamp*/)
{
	const int stampsize = 80;
	char stampbuf[stampsize];
	return os << Timestamp::GetStamp(stampbuf, stampsize, true);
}
