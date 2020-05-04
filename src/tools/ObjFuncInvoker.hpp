#pragma once

#include <list>

template<typename T>
class ObjFuncInvoker
{
	typedef void (T::*Func)();
	struct FuncItem
	{
		int last_ms;                    // 倒计时（毫秒）
		Func func;                      // 回调函数
	};

public:
	ObjFuncInvoker(T *obj) :
		m_obj(obj)
	{
	}
	ObjFuncInvoker(const ObjFuncInvoker &) = delete;
	ObjFuncInvoker &operator=(const ObjFuncInvoker &) = delete;

	// 注册回调函数
	void Register(Func f, int last_ms = 0)
	{
		for (const auto &item : m_func_set)
		{
			if (item.func == f)
			{
				return;
			}
		}

		FuncItem func_item {last_ms, f};
		m_func_set.push_back(func_item);
	}

	// 延迟呼叫，倒计时到了才触发回调函数
	void CheckInvoke(int interval_ms)
	{
		if (m_func_set.size() == 0)
		{
			return;
		}

		for (auto it = m_func_set.begin(); it != m_func_set.end();)
		{
			it->last_ms -=  interval_ms;
			if (it->last_ms <= 0)
			{
				(m_obj->*(it->func))();
				it = m_func_set.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// 立即呼叫，调用所有回调函数
	void Invoke()
	{
		if (m_func_set.size() == 0)
		{
			return;
		}

		for (auto &item : m_func_set)
		{
			(m_obj->*(item.func))();
		}
		m_func_set.clear();
	}

private:
	T *m_obj;
	std::list<FuncItem> m_func_set;
};
