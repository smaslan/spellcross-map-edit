#pragma once

#include <cstdint>

namespace cxxmidi {
    namespace guts {

        class FileData
        {
        public:
            uint8_t* data;
            int len;
            uint8_t* ptr;
            uint8_t* end;

            void set(uint8_t* data,int len)
            {
                this->data = data;
                this->len = len;
                this->end = &data[len];
                this->ptr = data;
            };
            FileData(uint8_t* data,int len)
            {
                set(data,len);
            };
            void rewind()
            {
                ptr = data;
            }
            int compare(const char *str, int size)
            {
                if(&ptr[size] > end)
                    return(false);
                return(std::memcmp(ptr,str,size) == 0);
            }
            int good(int count=0)
            {
                if(count == 0)
                    return(ptr < end);
                return(&ptr[count] <= end);
            }
        };
    };
};