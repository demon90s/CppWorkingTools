#include "serializer.hpp"
#include "tools/buffer.hpp"

#include <memory>

Serializer& operator<<(Serializer &s, const std::string &v)
{
	int len = static_cast<int>(v.length());
	s << len;
	s.WriteData(v.c_str(), v.length());
	return s;
}

DeSerializer& operator>>(DeSerializer &d, std::string &v)
{
	int len;
	d >> len;

    Buffer buffer;
    while (buffer.Size() < len)
    {
        buffer.Reallocate();
    }

    d.ReadData(buffer.Data(), len);
	v = std::string(buffer.Data(), len);

	return d;
}
