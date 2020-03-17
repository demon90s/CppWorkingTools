#include "utf8check.hpp"
#include <string>
#include <fstream>

bool IsUTF8(const char* buffer, long size)
{
	bool is_utf8 = true;
	unsigned char* start = (unsigned char*)buffer;
	unsigned char* end = (unsigned char*)buffer + size;
	while (start < end)
	{
		if (*start < 0x80)
		{
			start++;
		}
		else if (*start < (0xC0))  
		{
			is_utf8 = false;
			break;
		}
		else if (*start < (0xE0))
		{
			if (start >= end - 1)
				break;
			if ((start[1] & (0xC0)) != 0x80)
			{
				is_utf8 = false;
				break;
			}
			start += 2;
		}
		else if (*start < (0xF0))
		{
			if (start >= end - 2)
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				is_utf8 = false;
				break;
			}
			start += 3;
		}
		else
		{
			is_utf8 = false;
			break;
		}
	}

	return is_utf8;
}

bool IsUTF8_Bom(const char* buffer, long size)
{
	if (!IsUTF8(buffer, size)) return false;

	if (static_cast<unsigned char>(buffer[0]) == 0xEF &&
		static_cast<unsigned char>(buffer[1]) == 0xBB &&
		static_cast<unsigned char>(buffer[2]) == 0xBF)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Is_File_UTF8_NoBom(const char* filename)
{
	std::ifstream ifs(filename);

	if (!ifs)
	{
		fprintf(stderr, "open file failed\n");
		return false;
	}

	std::string file;
	std::string line;
	while (std::getline(ifs, line))
	{
		file += line;
		file += '\n';
	}

	bool is_utf8_no_bom = IsUTF8(file.c_str(), static_cast<long>(file.length())) && !IsUTF8_Bom(file.c_str(), static_cast<long>(file.length()));

	return is_utf8_no_bom;
}
