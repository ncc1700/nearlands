#include "apic.h"
#include "nrstatus.h"
#include <ki/cpu.h>











NearStatus KiWakeUpAllProcessors(){
    u64 apicAddr = KiReturnAPICAddress();
}