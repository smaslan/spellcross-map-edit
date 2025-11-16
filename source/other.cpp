#include "other.h"

#include <string>
#include <codecvt>
#include <filesystem>
#include <random>


std::wstring char2wstring(const char* str)
{
    //setup converter
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type,wchar_t> converter;
    return converter.from_bytes(str);
}

std::wstring string2wstring(std::string str)
{
    return char2wstring(str.c_str());
}

// convert wstring to UTF-8 string
std::string wstring2string(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

// convert char str to wstring using CZ CP852 encoding
std::wstring char2wstringCP852(const char* str)
{
    std::wstring codes = {L"ÇüéâäůćçłëŐőîŹÄĆ"
                     L"ÉĹĺôöĽľŚśÖÜŤťŁ×č"
                     L"áíóúĄąŽžĘę¬źČş«»"
                     L"░▒▓│┤ÁÂĚŞ╣║╗╝Żż┐"
                     L"└┴┬├─┼Ăă╚╔╩╦╠═╬¤"
                     L"đĐĎËďŇÍÎě┘┌█▄ŢŮ▀"
                     L"ÓßÔŃńňŠšŔÚŕŰýÝţ´"
                     L" ˝˛ˇ˘§÷¸°¨˙űŘř■ "};
    
    std::wstring result;
    result.resize(strlen(str));
    wchar_t *data = result.data();
    while(*str)
    {
        int code = (unsigned)(unsigned char)*str++;
        wchar_t wcode = code;
        if(code >= 128)
            wcode = codes.data()[code-128];
        *data++ = wcode;
    }
    return(result);
}

// convert char str to wstring using CZ CP895 encoding
std::wstring char2wstringCP895(const char* str)
{
    std::wstring codes ={ L"ČüéďäĎŤčěĚĹÍľĺÄÁ"
                     L"ÉžŽôöÓůÚýÖÜŠĽÝŘť"
                     L"áíóúňŇŮÔšřŕŔ¼§«»"
                     L"░▒▓│┤╡╢╖╕╣║╗╝╜╛┐"
                     L"└┴┬├─┼╞╟╚╔╩╦╠═╬╧"
                     L"╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀"
                     L"αßΓπΣσµτΦΘΩδ∞φε∩"
                     L"≡±≥≤⌠⌡÷≈°∙·√ⁿ²■ "};

    std::wstring result;
    result.resize(strlen(str));
    wchar_t* data = result.data();
    while(*str)
    {
        int code = (unsigned)(unsigned char)*str++;
        wchar_t wcode = code;
        if(code >= 128)
            wcode = codes.data()[code-128];
        *data++ = wcode;
    }
    return(result);
}

// convert unicode symbol to char using CP895 table
char wchar2charCP895(wchar_t sym)
{
    std::wstring codes = {L"ČüéďäĎŤčěĚĹÍľĺÄÁ"
                     L"ÉžŽôöÓůÚýÖÜŠĽÝŘť"
                     L"áíóúňŇŮÔšřŕŔ¼§«»"
                     L"░▒▓│┤╡╢╖╕╣║╗╝╜╛┐"
                     L"└┴┬├─┼╞╟╚╔╩╦╠═╬╧"
                     L"╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀"
                     L"αßΓπΣσµτΦΘΩδ∞φε∩"
                     L"≡±≥≤⌠⌡÷≈°∙·√ⁿ²■ "};
    if(sym < 128)
        return(char(sym));
    int unisym = codes.find(sym);
    if(unisym == std::wstring::npos)
        return(' ');
    return(128 + unisym);
}

// convert wstring using CZ CP895 encoding to string
std::string wstring2stringCP895(std::wstring str)
{
    std::string result;
    result.assign(str.size(),'\0');
    for(int k = 0; k < str.size(); k++)        
        result[k] = wchar2charCP895(str[k]);
    return(result);
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
void strcpy_noext(const char *dest, const char *src)
{
    char *ptr = (char*)dest;
    while(*src && *src != '.')
        *ptr++ = *src++;
    *ptr = '\0';
}

// convert single hex digit to int
int hex2num(char hex)
{
    if(hex >= '0' && hex <= '9')
        return(hex - '0');
    if(hex >= 'A' && hex <= 'F')
        return(hex - 'A' + 10);
    return(-1);
}
// convert single digit num to ascii char
char num2hex(int num)
{
    if(num >= 0 && num <= 9)
        return(num + '0');
    if(num >= 0x0A && num <= 0x0F)
        return(num - 0x0A + 'A');
    return('_');
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

// compare strings case insensitive
bool iequals(const std::string& a,const std::string& b)
{
    return std::equal(a.begin(),a.end(),
        b.begin(),b.end(),
        [](char a,char b) {
            return std::tolower(a) == std::tolower(b);
        });
}


// write string with string size prefix (16bit)
int ostream_write_string(std::ofstream& fw,std::string &str)
{
    return(ostream_write_string(fw,str.c_str()));
}
int ostream_write_string(std::ofstream& fw,std::string str)
{
    return(ostream_write_string(fw, str.c_str()));
}
int ostream_write_string(std::ofstream& fw, const char *str)
{
    uint16_t len = std::strlen(str) + 1;
    fw.write((char*)&len,sizeof(uint16_t));
    fw.write(str,len);
    return(0);
}

// read string item with size prefix (16bit)
std::string istream_read_string(std::ifstream& fr)
{
    uint16_t len;
    fr.read((char*)&len,sizeof(uint16_t));
    std::string str(len,'\0');
    //str.reserve(len + 1);
    fr.read(str.data(),len);
    str.resize(len-1);
    return(str);
}

// write u8 value
int ostream_write_u8(std::ofstream& fw,uint8_t val)
{
    fw.write((char*)&val,sizeof(uint8_t));
    return(0);
}

// write u16 value
int ostream_write_u16(std::ofstream& fw,uint16_t val)
{
    fw.write((char*)&val,sizeof(uint16_t));
    return(0);
}

// write u32 value
int ostream_write_u32(std::ofstream& fw,uint32_t val)
{
    fw.write((char*)&val,sizeof(uint32_t));
    return(0);
}

// write i32 value
int ostream_write_i32(std::ofstream& fw,int32_t val)
{
    fw.write((char*)&val,sizeof(int32_t));
    return(0);
}

// read uint32_t
uint32_t istream_read_u32(std::ifstream& fr)
{
    uint32_t val;
    fr.read((char*)&val,sizeof(uint32_t));
    return(val);
}

// read int32_t
int32_t istream_read_i32(std::ifstream& fr)
{
    int32_t val;
    fr.read((char*)&val,sizeof(int32_t));
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

// apply gamma to given palette
void apply_gamma(uint8_t *pal, double gamma)
{
    for(int k = 0; k < 256*3; k++)
        *pal++ = (uint8_t)(pow((double)*pal / 255.0,1.0 / gamma) * 255.0);
}

// correct modulo
int mod(int x, int y) {
    if(y == -1) return 0;
    return x - y * (x / y - (x % y && (x ^ y) < 0));
}

// generate random number in range min-max, uniform distribution
double rand(double min,double max)
{
    static std::default_random_engine re((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> unif(min,max);
    return(unif(re));
}

// generate exponential random number with max value limit
double randexp(double pow,double limit)
{
    static std::default_random_engine re((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> distribution(pow);
    double num;
    do{
        num = distribution(re);
    }while(num > limit && limit > 1e-3);
    return(num);    
}

// generate gamma distribution number normalized, so maximum PDF is 1.0, with optional maximum output limits
double randgman(double shape,double scale,double max, double min)
{
    static std::default_random_engine re((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::gamma_distribution<double> distribution(shape,scale);
    
    // maximum of PDF
    double imode = 1.0/((shape - 1.0)*scale);
    
    // get random number lower than limit
    double num;
    do {
        num = distribution(re)*imode;
    } while(num > max || num < min);

    return(num);
}


