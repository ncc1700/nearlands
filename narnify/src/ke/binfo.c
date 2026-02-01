#include "bootinfo.h"
#include <ke\binfo.h>



static BootInfo* bInfo = NULL;




void KeStoreBootInformationAddress(BootInfo* info){
    bInfo = info;
}


BootInfo* KeReturnBootInformation(){
    return bInfo;
}