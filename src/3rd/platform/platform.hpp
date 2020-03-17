#pragma once

namespace platform
{
	// 挂起进程，可以选择输出一个字符串作为提示
	void Pause(const char* notice = nullptr);

	// 获取毫秒时间戳
	unsigned long long PITime();

	// 睡眠执行线程若干毫秒
	void PISleep(unsigned long timems);
}
