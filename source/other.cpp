#include "other.h"

#include <string>
#include <codecvt>

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