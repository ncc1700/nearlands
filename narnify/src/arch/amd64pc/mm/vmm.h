#ifndef VMM_H_INCLUDED
#define VMM_H_INCLUDED







#include "../../../types.h"
#include "../../../bootinfo.h"






boolean MmMapPages(u64 physAddr, u64 virtAddr, u64 flags);
boolean MmInitVirtualMemoryManager(BootMemoryMap* memMap);























#endif