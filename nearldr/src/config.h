#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED








#include "externheaders/efi/Uefi.h"
#include <stdint.h>


typedef struct _OsLog {
    char* name;
    char* kernel;
} OsLog;
typedef struct _Config { 
    int timeout;
    char* name;
    char* kernel;
} Config;

int config_parse_handler(void* user, const char* section, const char* name,
                   const char* value);




#endif