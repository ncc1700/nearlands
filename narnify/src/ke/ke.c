
#include "bootinfo.h"
#include "ke/binfo.h"
#include "ke/spinlock.h"
#include "nrstatus.h"
#include <ke/ke.h>
#include <ke/thread.h>








NearStatus KeInitSystem(BootInfo* info){
    NearStatus status = KeInitializeThreadComponent();
    if(!NR_SUCCESS(status)) return status;
    status = KeInitializeSpinlockComponent();
    if(!NR_SUCCESS(status)) return status;
    KeStoreBootInformationAddress(info);
    // do more initalization of the kernel
    return STATUS_SUCCESS;
}