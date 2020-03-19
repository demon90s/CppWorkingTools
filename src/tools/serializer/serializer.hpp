#pragma once

#include <cstring>
#include <stdexcept>

class Serializer
{
	static const size_t DEFAULT_LEN = 1024;

public:
	Serializer() : m_len(DEFAULT_LEN), m_rawbuffer{}, m_buffer(nullptr), m_cur_pos(m_rawbuffer) {}
	~Serializer() { if (m_buffer) delete []m_buffer; }

	// 当前序列化缓冲区中的数据长度
	size_t Size() const { return m_cur_pos - CurBuffer(); }
	
	// 缓冲区地址
	const char* Ptr() const { return CurBuffer(); }

	template<typename T>
	void Write(const T &v)
	{
		WriteData((const char*)&v, sizeof(v));
	}

	template<typename T, typename... Args>
	void Write(const T &v, const Args&... rest)
	{
		Write(v);
		Write(rest...);
	}

	void WriteData(const char *data, int len)
	{
		while (m_cur_pos - CurBuffer() + len > m_len)
		{
			ReAllocate();
		}
		memcpy(m_cur_pos, data, len);
		m_cur_pos += len;
	}

private:
	// 拒绝拷贝
	Serializer(const Serializer&) = delete;
	Serializer& operator=(const Serializer&) = delete;

	const char *CurBuffer() const
	{
		return m_buffer ? m_buffer : m_rawbuffer;
	}
	char *CurBuffer()
	{
		return const_cast<char*>(static_cast<const Serializer*>(this)->CurBuffer());
	}

	void ReAllocate()
	{
		char *new_buffer = new char[2 * m_len]{};
		memcpy(new_buffer, CurBuffer(), m_len);
		m_cur_pos = new_buffer + Size();
		delete[]m_buffer;
		m_buffer = new_buffer;
		m_len *= 2;
	}

	int m_len;						// buffer长度
	char m_rawbuffer[DEFAULT_LEN];	// 初始buffer，用完才用heap memory
	char *m_buffer;					// 用于存储序列化数据的缓冲区，自动增长
	char *m_cur_pos;				// 当前序列化的位置
};

class DeSerializer
{
public:
	enum class State { Fail = 0, Eof = 1 };

	DeSerializer(const char *buffer, size_t len) : 
		m_max_len(len),
		m_buffer(buffer),
		m_cur_pos(m_buffer),
		m_state_flag(0) {}

	explicit operator bool() const
	{
		return m_state_flag == 0;
	}

	template<typename T>
	void Read(T &v)
	{
		ReadData((char *)&v, sizeof(v));
	}

	template<typename T, typename... Args>
	void Read(T &v, Args&... rest)
	{
		Read(v);
		Read(rest...);
	}

	void ReadData(char *data, int len)
	{
		if (m_cur_pos - m_buffer == m_max_len)
		{
			this->SetState(State::Eof);
			return;
		}
		if (m_cur_pos - m_buffer + len > m_max_len)
		{
			throw std::overflow_error("DeSerializer::Read failed: no more data to read");
		}
		memcpy(data, m_cur_pos, len);
		m_cur_pos += len;
	}

	bool Eof() const { return m_state_flag & (1 << static_cast<int>(State::Eof)); }
	void SetState(State state) { m_state_flag |= (1 << static_cast<int>(state)); }
	void SetFailState() { m_state_flag |= (1 << static_cast<int>(State::Fail)); }

private:
	// 拒绝拷贝
	DeSerializer(const DeSerializer&) = delete;
	DeSerializer& operator=(const DeSerializer&) = delete;

	const int m_max_len;			// buffer最大长度
	const char *const m_buffer;		// 用于反序列化数据的缓冲区
	const char *m_cur_pos;			// 当前反序列化的位置
	unsigned int m_state_flag;		// 状态标记
};

inline Serializer& operator<<(Serializer &s, signed char v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, signed char &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, unsigned char v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, unsigned char &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, short v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, short &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, unsigned short v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, unsigned short &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, int v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, int &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, unsigned int v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer& d, unsigned int& v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, long long v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, long long &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, unsigned long long v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, unsigned long long &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, float v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, float& v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, double v)
{
	s.Write(v); return s;
}
inline DeSerializer& operator>>(DeSerializer &d, double &v)
{
	d.Read(v); return d;
}

inline Serializer& operator<<(Serializer &s, const char *v)
{
	int len = static_cast<int>(strlen(v));
	s << len;
	s.WriteData(v, len); 
	return s;
}
inline DeSerializer& operator>>(DeSerializer &d, char *v)
{
	int len;
	d.Read(len);
	d.ReadData(v, len);
	return d;
}

#include <string>
Serializer& operator<<(Serializer &s, const std::string &v);
DeSerializer& operator>>(DeSerializer &d, std::string &v);

#include <vector>
template<typename T>
Serializer& operator<<(Serializer &s, const std::vector<T> &v)
{
	int len = static_cast<int>(v.size());
	s << len;
	for (const auto &item : v)
	{
		s << item;
	}
	return s;
}

template<typename T>
DeSerializer& operator>>(DeSerializer &d, std::vector<T> &v)
{
	int len;
	d >> len;
	for (int i = 0; i < len; i++)
	{
		T item {};
		if (d >> item)
			v.push_back(item);
	}
	return d;
}

#include <map>
template<typename K, typename V>
Serializer& operator<<(Serializer &s, const std::map<K, V> &v)
{
	int len = static_cast<int>(v.size());
	s << len;
	for (const auto &p : v)
	{
		s << p.first << p.second;
	}
	return s;
}

template<typename K, typename V>
DeSerializer& operator>>(DeSerializer &d, std::map<K, V> &v)
{
	int len;
	d >> len;
	for (int i = 0; i < len; i++)
	{
		K key;
		V value;
		d >> key >> value;
		v[key] = value;
	}
	return d;
}
