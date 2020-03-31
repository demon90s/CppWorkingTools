#include "gtest/gtest.h"

#include "stringcommon/stringcommon.hpp"
TEST(stringcommon, Trim)
{
	std::string str = "hello";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello");

	str = " hello";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello");

	str = "   hello  ";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello");

	str = "";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "");

	str = "hello world   ";
	ASSERT_STREQ(stringcommon::Trim(str).c_str(), "hello world");
}

TEST(stringcommon, Format)
{
	std::string s = stringcommon::Format("hello, x=%d, y=%g", 42, 3.14);
	ASSERT_STREQ(s.c_str(), "hello, x=42, y=3.14");
}

TEST(stringcommon, Split)
{
	std::vector<std::string> splits = stringcommon::Split("hello:world:123:3.14", ":");
	ASSERT_EQ(static_cast<int>(splits.size()), 4);
	ASSERT_STREQ(splits[0].c_str(), "hello");
	ASSERT_STREQ(splits[2].c_str(), "123");

	splits = stringcommon::Split("1:2:3:", ":");
	ASSERT_EQ(splits.size(), 3u);

	splits = stringcommon::Split("::123:", ":");
	ASSERT_EQ(splits.size(), 3u);
}

TEST(stringcommon, FetchNameFromPath)
{
	std::string path = "/root/diwen/hello.txt";

	auto file = stringcommon::FetchNameFromPath(path);

	ASSERT_STREQ(file.c_str(), "hello.txt");
}