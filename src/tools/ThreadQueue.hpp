#include <mutex>
#include <list>

template<typename T>
class ThreadQueue {
public:
	void Push(const T &v) {
		std::lock_guard<std::mutex> m_lock_guard(m_lock);
		m_q.push_back(v);
	}

	bool Pop(T &v) {
		std::lock_guard<std::mutex> m_lock_guard(m_lock);
        if (m_q.size() == 0)
        {
            return false;
        }
		v = std::move(m_q.front());
		m_q.pop_front();
		return true;
	}

private:
	std::mutex m_lock;
	std::list<T> m_q;
};
