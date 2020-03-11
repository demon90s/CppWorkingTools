#include <iostream>
#include <sstream>
#include <cassert>

#include "stringcommon/stringcommon.hpp"
void Test_stringcommon()
{
	std::string str = "hello";
	assert(stringcommon::Trim(str) == "hello");
	
	str = " hello";
	assert(stringcommon::Trim(str) == "hello");

	str = "   hello  ";
	assert(stringcommon::Trim(str) == "hello");
}

#include "configstruct/attributepairconfig.hpp"
void Test_AttributePairConfig()
{
	{
		std::string config_str = "gongji=1000, fangyu=2000";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		assert(iss >> config);

		const auto& pair_list = config.GetPairList();
		assert(pair_list.size() == 2);

		assert(pair_list[0].attr_str == "gongji" && pair_list[0].attr_value == 1000);
		assert(pair_list[1].attr_str == "fangyu" && pair_list[1].attr_value == 2000);

	}
	
	{
		std::string config_str = "   gongji =  1000";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		assert(iss >> config);

		const auto& pair_list = config.GetPairList();
		assert(pair_list.size() == 1);

		assert(pair_list[0].attr_str == "gongji" && pair_list[0].attr_value == 1000);
	}

	{
		std::string config_str = "gongji= 1000,";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		assert(iss >> config);
	}

	{
		std::string config_str = "gongji=1000|";
		std::istringstream iss(config_str);

		AttributePairConfig config;
		assert(!(iss >> config));
	}
}

#include "configstruct/serveropendayconfig.hpp"
void Test_ServerOpenDayConfig()
{
	{
		std::string config_str = "1,7";
		std::istringstream iss(config_str);

		ServerOpenDayConfig config;
		assert(iss >> config);
		assert(config.IsInServerOpenDay(1));
		assert(config.IsInServerOpenDay(7));
		assert(config.IsInServerOpenDay(5));
		assert(!config.IsInServerOpenDay(10));
	}

	{
		std::string config_str = "1|7";
		std::istringstream iss(config_str);

		ServerOpenDayConfig config;
		assert(!(iss >> config));
	}
}

int main()
{
	Test_stringcommon();
	Test_AttributePairConfig();
	Test_ServerOpenDayConfig();

	std::cout << "ALL TEST PASS\n";

	std::cin.get();
}
