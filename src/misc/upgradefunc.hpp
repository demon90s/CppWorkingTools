#pragma once

#include <vector>

void Upgrade(long long &cur_exp, long long inc_exp, int &cur_level,
            const std::vector<long long> &upgrade_exp_list)
{
    if (inc_exp <= 0 || cur_level < 0)
    {
        return;
    }

    cur_exp += inc_exp;

    while (cur_exp >= upgrade_exp_list[cur_level] &&
            cur_level < static_cast<int>(upgrade_exp_list.size()))
    {
        cur_exp -= upgrade_exp_list[cur_level];
        cur_level++;
    }
}