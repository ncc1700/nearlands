#ifndef NPRINTFIMPL_H_INCLUDED
#define NPRINTFIMPL_H_INCLUDED













#include <stdarg.h>
#include "../../types.h"


int impl_vsnprintf(char *const buffer, const size_t bufferCount, const char *const format, va_list argList);





#endif