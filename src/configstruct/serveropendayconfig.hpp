#pragma once

#include <iostream>

// ����ʱ������
// ��ʽ��begin_time,end_time
// ���ӣ�1,7

struct ServerOpenDayConfig
{
	friend std::istream& operator>>(std::istream& stream, ServerOpenDayConfig& config);

	bool IsInServerOpenDay(int server_open_day) const
	{
		return begin_day <= server_open_day && server_open_day <= end_day;
	}
private:
	int begin_day = 0;
	int end_day = 0;
};

std::istream& operator>>(std::istream& stream, ServerOpenDayConfig& config);