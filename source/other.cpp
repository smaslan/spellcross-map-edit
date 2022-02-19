#include "other.h"

#include <string>
#include <codecvt>
#include <filesystem>

std::wstring char2wstring(const char* str)
{
    //setup converter
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type,wchar_t> converter;
    return converter.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring2string(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

// compare wildcard string to string
int wildcmp(const char* wild,const char* string)
{
    // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    const char* cp = NULL,* mp = NULL;

    while((*string) && (*wild != '*')) {
        if((*wild != *string) && (*wild != '?')) {
            return 0;
        }
        wild++;
        string++;
    }

    while(*string) {
        if(*wild == '*') {
            if(!*++wild) {
                return 1;
            }
            mp = wild;
            cp = string+1;
        }
        else if((*wild == *string) || (*wild == '?')) {
            wild++;
            string++;
        }
        else {
            wild = mp;
            string = cp++;
        }
    }

    while(*wild == '*') {
        wild++;
    }
    return !*wild;
}

// copy string with no extension, assuming destiantion has enough space!
void strcpy_noext(char *dest, char *src)
{
    while(*src && *src != '.')
        *dest++ = *src++;
    *dest = '\0';
}

// convert single hex digit to int
int hex2num(char hex)
{
    if(hex >= '0' && hex <= '9')
        return(hex - '0');
    else if(hex >= 'A' && hex <= 'F')
        return(hex - 'A' + 10);
    else
        return(-1);
}


std::string string_format(const std::string fmt,...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while(1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap,fmt);
        int n = vsnprintf((char*)str.data(),size,fmt.c_str(),ap);
        va_end(ap);
        if(n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if(n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

// write string with string size prefix (16bit)
int ostream_write_string(ofstream& fw,std::string &str)
{
    return(ostream_write_string(fw,str.c_str()));
}
int ostream_write_string(ofstream& fw,std::string str)
{
    return(ostream_write_string(fw, str.c_str()));
}
int ostream_write_string(ofstream& fw, const char *str)
{
    uint16_t len = std::strlen(str) + 1;
    fw.write((char*)&len,sizeof(uint16_t));
    fw.write(str,len);
    return(0);
}

// read string item with size prefix (16bit)
std::string istream_read_string(ifstream& fr)
{
    uint16_t len;
    fr.read((char*)&len,sizeof(uint16_t));
    std::string str(len,'\0');
    //str.reserve(len + 1);
    fr.read(str.data(),len);
    str.resize(len-1);
    return(str);
}

// write u32 value
int ostream_write_u32(ofstream& fw,uint32_t val)
{
    fw.write((char*)&val,sizeof(uint32_t));
    return(0);
}

// read uint32_t
uint32_t istream_read_u32(ifstream& fr)
{
    uint32_t val;
    fr.read((char*)&val,sizeof(uint32_t));
    return(val);
}



// write u32 to ofstream
/*basic_ofstream<charT,Traits> &write(uint32_t data)
{
    this->write((char*)&data, sizeof(uint32_t));
}*/



// Bresneham algorithm (based on GitHub/bert algorithm)
void plot_line(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y,int x_size,uint8_t color,int x0,int y0,int x1,int y1)
{
    int dx =  abs(x1 - x0),sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0),sy = y0 < y1 ? 1 : -1;
    int err = dx + dy,e2; /* error value e_xy */

    for(;;) {  /* loop */
        uint8_t* pix = &buffer[buf_x + x0 + (buf_y + y0) * x_size];
        if(pix >= buffer && pix < buf_end)
            *pix = color;
        if(x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if(e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if(e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}
