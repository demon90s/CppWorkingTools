#include "traceback.hpp"

#ifdef _WIN32
#include "stackwalker.hpp"
#else
#include "stacktrace.hpp"
#endif
#include <string>
#include <vector>
#include <cstdio>
#include "stringcommon/stringcommon.hpp"

void TraceBack()
{
	std::string out = GetTraceBack();
	fprintf(stderr, "%s", out.c_str());
}

std::string GetTraceBack()
{
	std::string out;
#ifdef _MSC_VER
	StackWalker sw;
	sw.ShowCallstack(&out);
#else
	stacktrace(out);
#endif
	std::vector<std::string> frames = stringcommon::Split(out, "\n");

	std::string traceback_str;
	for (int i = 0; i < static_cast<int>(frames.size()); ++i)
	{
		char buffer[1024] {};
		snprintf(buffer, sizeof(buffer), "#%d: %s\n", i, frames[i].c_str());

		traceback_str += buffer;
	}

	return traceback_str;
}
