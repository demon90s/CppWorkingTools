#include <string>

static std::string& TrimString(std::string& str)
{
	if (str.empty())
	{
		return str;
	}

	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);

	return str;
}

#include "attributepairconfig.hpp"
std::istream& operator>>(std::istream& stream, AttributePairConfig& config)
{
	do {
		AttributePairConfig::PairItem pair_item;

		if (!std::getline(stream, pair_item.attr_str, '='))
		{
			break;
		}

		if (!(stream >> pair_item.attr_value))
		{
			break;
		}

		TrimString(pair_item.attr_str);
		config.pair_list.push_back(pair_item);

		char tmp;
		if (!(stream >> tmp))
		{
			break;
		}

		if (tmp != ',')
		{
			stream.clear(std::ios_base::failbit);
			break;
		}
	} while (true);

	if (stream.eof())
	{
		stream.clear();
	}

	return stream;
}

#include "serveropendayconfig.hpp"
std::istream& operator>>(std::istream& stream, ServerOpenDayConfig& config)
{
	char tmp;
	if (stream >> config.begin_day&& stream >> tmp && stream >> config.end_day)
	{
		if (tmp != ',')
		{
			stream.clear(std::ios_base::failbit);
		}
	}
	return stream;
}