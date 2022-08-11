#ifndef other_H
#define other_H

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <vector>

//using namespace std;

std::wstring char2wstring(const char* str);
std::string wstring2string(const std::wstring& str);
std::wstring char2wstringCP852(const char* str);
std::wstring char2wstringCP895(const char* str);
char wchar2charCP895(wchar_t sym);
std::string wstring2stringCP895(std::wstring str);
std::string string_format(const std::string fmt,...);
bool iequals(const std::string& a,const std::string& b);

int wildcmp(const char* wild,const char* string);
void strcpy_noext(const char* dest,const char* src);
int hex2num(char hex);
char num2hex(int num);

//int ostream_write_string(ofstream& fw,std::string &str);
int ostream_write_string(std::ofstream& fw,std::string str);
int ostream_write_string(std::ofstream& fw,const char* str);
std::string istream_read_string(std::ifstream& fr);
int ostream_write_u32(std::ofstream& fw,uint32_t val);
int ostream_write_i32(std::ofstream& fw,int32_t val);
uint32_t istream_read_u32(std::ifstream& fr);
int32_t istream_read_i32(std::ifstream& fr);


void plot_line(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y,int x_size,uint8_t color,int x0,int y0,int x1,int y1);

void apply_gamma(uint8_t* pal,double gamma);

int mod(int x,int y);
double rand(double min,double max);
double randexp(double pow,double limit=1e9);
double randgman(double shape,double scale,double max=1e9,double min=0);

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


