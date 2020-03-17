#include "misc.hpp"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <stdexcept>

bool IsNumber(const char* v, double* d)
{
    char* end;
    double res = strtod(v, &end);
    if (*end == '\0')
    {
        if (d) *d = res;
        return true;
    }
    return false;
}

int RandomNum(int beg, int end)
{
	if (beg > end)
	{
		throw std::logic_error("RandomNum param error");
	}

	int interval_num = end - beg;
	if (interval_num == 0)
	{
		return 0;
	}

	if (interval_num < RAND_MAX)
	{
		return beg + (rand() % interval_num);
	}
	else
	{
		int rand_num = (rand() << 16) + rand();
		if (rand_num < 0)
		{
			rand_num *= -1;
		}
		return beg + static_cast<int>(rand_num % interval_num);
	}
}

long long ConvertParamToLongLong(int param_0, int param_1)
{
	union
	{
		long long ll_value;
		struct
		{
			int param_0;
			int param_1;
		};
	} union_param;

	union_param.param_0 = param_0;
	union_param.param_1 = param_1;

	return union_param.ll_value;
}

void ParseParamFromLongLong(long long key, int* param_0, int* param_1)
{
	union
	{
		long long ll_value;
		struct
		{
			int param_0;
			int param_1;
		};
	} union_param;

	union_param.ll_value = key;
	*param_0 = union_param.param_0;
	*param_1 = union_param.param_1;
}

bool TimeStrToTimestamp(const char* time_str, time_t* timestamp)
{
	if (NULL == time_str || NULL == timestamp) return false;

	if (strlen(time_str) < 19) return false;

	char temp[256] = { 0 };
	strncpy(temp, time_str, sizeof(temp));

	struct tm local_tm;
	memset(&local_tm, 0, sizeof(local_tm));

	char* pt = &temp[0];

	pt[4] = '\0';
	local_tm.tm_year = atoi(pt) - 1900; if (local_tm.tm_year <= 70 || local_tm.tm_year > 200) return false;
	pt += 5;

	pt[2] = '\0';
	local_tm.tm_mon = atoi(pt) - 1; if (local_tm.tm_mon < 0 || local_tm.tm_mon > 11) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_mday = atoi(pt); if (local_tm.tm_mday < 1 || local_tm.tm_mday > 31) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_hour = atoi(pt); if (local_tm.tm_hour < 0 || local_tm.tm_hour > 23) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_min = atoi(pt); if (local_tm.tm_min < 0 || local_tm.tm_min > 59) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_sec = atoi(pt); if (local_tm.tm_sec < 0 || local_tm.tm_sec > 59) return false;
	pt += 3;

	*timestamp = mktime(&local_tm);

	return true;
}