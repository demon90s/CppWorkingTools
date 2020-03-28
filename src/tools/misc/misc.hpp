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
    if (iss >> val)
    {
        char c;
        if ((iss >> c).eof()) return true;
    }
    return false;
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

// 判断v是否是一个数字，如果是，将其拷贝到d所绑定的空间
bool IsNumber(const char* v, double* d = nullptr);

// 取随机整数值，取值范围是[beg, end)
int RandomNum(int beg, int end);

// 获取随机项，每个项目提供一个获取其权重的方法
#include <functional>
template<typename Container>
auto GetRandItem(const Container &draw_pool, std::function<int(decltype(*std::begin(draw_pool)))> get_item_weight)
    -> decltype(std::begin(draw_pool))
{
    int total_weight = 0;
    for (const auto &item : draw_pool)
    {
        total_weight += get_item_weight(draw_item);
    }

    int rand_num = RandomNum(total_weight);
    auto it = std::begin(draw_pool);
    for (; it != std::end(draw_pool); ++it)
    {
        int item_weight = get_item_weight(*it);
        if (rand_num < item_weight)
        {
            break;
        }
        else
        {
            rand_num -= item_weight;
        }
    }
    return it;
}

// 两个int与一个long long之间的转换
long long ConvertParamToLongLong(int param_0, int param_1);
void ParseParamFromLongLong(long long key, int* param_0, int* param_1);

// 时间字符串（格式如 2018-09-16 10:12:08）转换成时间戳
bool TimeStrToTimestamp(const char* time_str, time_t* timestamp);