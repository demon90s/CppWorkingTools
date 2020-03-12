#pragma once

#include <ctime>
#include <cstdio>
#include <string>

template<time_t (*NowFunc)(time_t *) = time>
class TimeUtil
{
public:
    static long long NowTimestamp()
    {
        return NowFunc(NULL);
    }

    static std::string SecondToString(long long second)
    {
        if (second <= 0)
        {
            return "00:00:00";
        }
        int hour = second / 3600;
		int minutes = (second - hour * 3600) / 60;
		int sec = second - hour * 3600 - minutes * 60;

        char buffer[1024] {};
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hour, minutes, sec);

        return buffer;
    }

    static const char* week_name[7];

    class Timestamp
    {
    private:
        long long m_timestamp;
    public:
        Timestamp(long long timestamp = 0) :
            m_timestamp(timestamp)
        {}
        operator long long()
        {
            return m_timestamp;
        }

        int Second() const
        {
            return LocalTime()->tm_sec;
        }
        int Minutes() const
        {
            return LocalTime()->tm_min;
        }
        int Hour() const
        {
            return LocalTime()->tm_hour;
        }
        int Day() const
        {
            return LocalTime()->tm_mday;
        }
        int Week() const
        {
            return LocalTime()->tm_wday;
        }
        int Month() const
        {
            return LocalTime()->tm_mon + 1;
        }
        int Year() const
        {
            return LocalTime()->tm_year + 1900;
        }

        std::string AscTime() const
        {
            char buffer[1024] {};
            snprintf(buffer, sizeof(buffer), "%d-%02d-%02d %02d:%02d:%02d %s",
                    Year(), Month(), Day(), Hour(), Minutes(), Second(), week_name[Week()] );
            return buffer;
        }

    private:
        const tm *LocalTime() const
        {
            time_t t = static_cast<time_t>(m_timestamp);
            return localtime(&t);
        }
    };
};

template<time_t (*NowFunc)(time_t *)>
const char* TimeUtil<NowFunc>::week_name[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", };