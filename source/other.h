#ifndef other_H
#define other_H

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdarg.h>

using namespace std;

std::wstring char2wstring(const char* str);
std::string wstring2string(const std::wstring& str);
std::string string_format(const std::string fmt,...);

int wildcmp(const char* wild,const char* string);
int hex2num(char hex);





/*template <class charT,class Traits>
class ofstreamext : public basic_ofstream<charT,Traits> {
public:
    ofstreamext(const char* name)
        : basic_ofstream<charT,Traits>(name) {}
    
    basic_ofstream<charT,Traits>& write(uint32_t data){
        write(
    };

};*/

/*class ofstreamext : public basic_ofstream<char,char_traits<char>>
{
public:
    void write(uint32_t u32);
};*/

// write numeric
/*void fwrite_num(ofstream &fw, uint32_t u32)
{
    fw.write((char*)&u32, sizeof(uint32_t));
}
void fwrite_num(ofstream& fw,uint16_t u16)
{
    fw.write((char*)&u16,sizeof(uint16_t));
}
// write string with size prefix
void fwrite_string(ofstream& fw,string str)
{
    uint16_t len = str.size()+1;
    fw.write((char*)&len,sizeof(uint16_t));
    fw.write((char*)str.c_str(),len);
}*/


#endif


