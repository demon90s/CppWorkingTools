#pragma once

#include "ThreadQueue.hpp"

#include <string>
#include <memory>
#include <thread>
#include <cstdarg>

class Logger
{
public:
    Logger() : m_run(true), m_thread(&Logger::WriteThread, this)
    {
    }

    ~Logger()
    {
        m_run = false;
        m_thread.join();
    }

    void Printf(const char *fmt, ...)
    {
        static const long long INIT_LEN = 1024;

		long long buffer_len = INIT_LEN;
		std::unique_ptr<char[]> buffer;
		char rawbuffer[INIT_LEN]{};
		bool use_raw = true;
		int str_len = -1;

		do
		{
			va_list ap;
			va_start(ap, fmt);

			str_len = use_raw ? vsnprintf(rawbuffer, buffer_len, fmt, ap) : 
								vsnprintf(buffer.get(), buffer_len, fmt, ap);

			if (str_len >= buffer_len) {
				str_len = -1;

				buffer = std::unique_ptr<char[]>(new char[buffer_len * 2]{});
				buffer_len *= 2;
				use_raw = false;
			}
			va_end(ap);
		} while (str_len == -1);

		m_log_queue.Push(use_raw ? rawbuffer : buffer.get());
    }

private:
    void WriteThread()
    {
        while (m_run)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            if (m_log_queue.Pop(m_msg))
            {
                fprintf(stdout, "%s\n", m_msg.c_str());
            }
        }
    }

    bool m_run;
    std::thread m_thread;
    std::string m_msg;
    ThreadQueue<std::string> m_log_queue;
};
