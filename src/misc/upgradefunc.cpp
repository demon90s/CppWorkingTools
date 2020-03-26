#include "upgradefunc.hpp"

bool Upgrade(long long &cur_exp, long long inc_exp, int &cur_level,
            const std::vector<long long> &upgrade_exp_list)
{
    if (inc_exp <= 0 || cur_level < 0)
    {
        return false;
    }

    if (upgrade_exp_list.size() < 1)
    {
        return false;
    }

    const int MAX_LEVEL = static_cast<int>(upgrade_exp_list.size()) - 1;

    if (cur_level >= MAX_LEVEL)
    {
        return false;
    }

    cur_exp += inc_exp;

    while (cur_level < MAX_LEVEL &&
        cur_exp >= upgrade_exp_list[cur_level])
    {
        cur_exp -= upgrade_exp_list[cur_level];
        cur_level++;
    }

    return true;
}