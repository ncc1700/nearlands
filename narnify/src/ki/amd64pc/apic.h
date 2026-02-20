#ifndef APIC_H_INCLUDED
#define APIC_H_INCLUDED








#include <types.h>





NearStatus KiInitApic();
u64 KiReturnAPICAddress();
u64 KiApicIoRead(u64 ioAddr, u32 reg);
void KiApicIoWrite(u64 ioAddr, u32 reg, u32 value);
u64 KiApicRead(u64 offset);
void KiApicWrite(u64 offset, u32 value);











#endif