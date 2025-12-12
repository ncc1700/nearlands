#include "config.h"
#include <stdint.h>
#include "string.h"


#define GLOBAL_CONFIG_STRING_MAX 64


int config_parse_handler(void* user, const char* section, const char* name,
                   const char* value)
{
    Config* pconfig = (Config*)user;
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("LoaderConfig", "pause")) {
        pconfig->pause = atoi(value);
    } else if (MATCH("LoaderConfig", "name")) {
        pconfig->name = strdup(value);
    } else if (MATCH("LoaderConfig", "kernel")) {
        pconfig->kernel = strdup(value);
    } else {
        return 0;
    }
    return 1;
}
