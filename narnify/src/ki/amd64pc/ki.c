#include "nrstatus.h"
#include <ki/ki.h>
#include "apic.h"






NearStatus KiInitSystem(){
    return KiInitApic();
}


