#pragma once

#include <string>
#include <vector>

namespace stringcommon
{
	std::string& Trim(std::string& str);

	std::string Format(const char* fmt, ...);

	std::vector<std::string> Split(const std::string& str, const char* delim);
}
