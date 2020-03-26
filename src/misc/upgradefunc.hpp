#pragma once

#include <vector>

bool Upgrade(long long &cur_exp, long long inc_exp, int &cur_level,
    const std::vector<long long> &upgrade_exp_list);