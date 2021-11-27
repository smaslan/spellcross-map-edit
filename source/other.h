#ifndef other_H
#define other_H

#include <string>

std::wstring char2wstring(const char* str);
std::string wstring2string(const std::wstring& str);

int wildcmp(const char* wild,const char* string);
int hex2num(char hex);

#endif


