#pragma once

// in c++ 17, you can use std::size
template <typename T, int N>
inline int ArrayItemCount(T(&arr)[N])
{
	return N;
}

#include <string>
#include <sstream>
template<typename T>
bool StringToType(const std::string &str, T &val)
{
    std::istringstream iss(str);
    return (iss >> val).eof();
}
template<typename T>
bool TypeToString(const T &val, std::string &str)
{
    std::ostringstream oss;
    if (oss << val) {
        str = std::move(oss.str());
        return true;
    }
    return false;
}

#include <cstdlib>
inline bool IsNumber(const char *v, double *d = nullptr)
{
    char *end;
    double res = strtod(v, &end);
    if (*end == '\0')
    {
        if (d) *d = res;
        return true;
    }
    return false;
}
