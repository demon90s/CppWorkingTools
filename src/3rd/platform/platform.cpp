#include "platform.hpp"
#include <iostream>

#ifdef __unix
#include <unistd.h>
#include <sys/time.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

namespace platform
{
	void Pause(const char* notice)
	{
		if (notice)
		{
			std::cout << notice << std::flush;
		}
#ifdef __unix
		pause();
#endif

#ifdef _MSC_VER
		system("pause > nul");
#endif
	}

	unsigned long long PITime()
	{
#ifdef __unix
		struct timezone tz = { 0, 0 };
		timeval time;
		gettimeofday(&time, &tz);
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
#elif defined(_WIN32)
		return GetTickCount64();
#endif
	}

	void PISleep(unsigned long timems)
	{
#ifdef __unix 	// usleep( time * 1000 );
		usleep((timems << 10) - (timems << 4) - (timems << 3));
#elif defined(_WIN32)
		Sleep(timems);
#endif
	}
}