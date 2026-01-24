#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"



int impl_vsnprintf(char *const buffer, const size_t bufferCount, const char *const format, va_list argList){
    return npf_vsnprintf(buffer, bufferCount, format, argList);
}

void NNPFFormatBuffer(char* buffer, char* str, ...){
    va_list arg;
    va_start(arg, str);
    impl_vsnprintf(buffer, 128, 
                    str, arg);
    va_end(arg);
}