#pragma once

#include <string>
#include <vector>
#include <iostream>

// 角色属性配置
// 格式；attr_str=attr_value[, ...]
// 例子：gongji=1000, fangyu=2000

struct AttributePairConfig
{
	friend std::istream& operator>>(std::istream& stream, AttributePairConfig& config);

	struct PairItem
	{
		std::string attr_str;
		long long attr_value;
	};

	const std::vector<PairItem>& GetPairList() const
	{
		return pair_list;
	}

private:
	std::vector<PairItem> pair_list;
};

std::istream& operator>>(std::istream& stream, AttributePairConfig& config);
