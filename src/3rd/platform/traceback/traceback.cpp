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
	std::string out;
#ifdef _MSC_VER
	StackWalker sw;
	sw.ShowCallstack(&out);
#else
	stacktrace(out);
#endif
	std::vector<std::string> frames = stringcommon::Split(out, "\n");

	for (int i = 0; i < static_cast<int>(frames.size()); ++i)
	{
		printf("#%d: %s\n", i, frames[i].c_str());
	}
}
