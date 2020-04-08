#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>

class Property
{
	struct PropertyItem
	{
		std::ostringstream oss;
		std::string name;
	};
public:
	template<typename T>
	void SetProperty(const std::string &key, const T &v)
	{
		PropertyItem &item = m_property[key];
		if (!item.name.empty())
		{
			if (typeid(v).name() != item.name)
			{
				std::cerr << "SetProperty " << key << " fail, type not equal" << std::endl;
				return;
			}
		}
		else
		{
			item.name = typeid(v).name();
		}

		item.oss.str("");
		item.oss << v;
	}

	template<typename T>
	T GetProperty(const std::string &key, const T &default_v = T{})
	{
		auto it = m_property.find(key);
		if (it == m_property.end())
		{
			return default_v;
		}

		if (it->second.name != typeid(default_v).name())
		{
			return default_v;
		}

		std::istringstream iss(it->second.oss.str());
		T v;
		iss >> v;
		return v;
	}

private:
	std::unordered_map<std::string, PropertyItem> m_property;
};
