#pragma once

// 判断 Buffer 是否是 UTF8 格式的序列
bool IsUTF8(const char* buffer, long size);

// 判断 Buffer 是否是 UTF8 带 BOM 格式的序列
bool IsUTF8_Bom(const char* buffer, long size);

// 判断文件是否是 UTF-8 无 BOM 格式的
bool Is_File_UTF8_NoBom(const char* filename);
