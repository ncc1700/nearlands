#ifndef BINFO_H_INCLUDED
#define BINFO_H_INCLUDED






#include <bootinfo.h>





void KeStoreBootInformationAddress(BootInfo* info);
BootInfo* KeReturnBootInformation();










#endif