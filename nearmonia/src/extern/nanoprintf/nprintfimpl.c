#define NANOPRINTF_IMPLEMENTATION
#include <nanoprintf/nanoprintf.h>



int impl_vsnprintf(char *const buffer, const size_t bufferCount, const char *const format, va_list argList){
    return npf_vsnprintf(buffer, bufferCount, format, argList);
}