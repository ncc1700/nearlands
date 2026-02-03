
#include "bootinfo.h"
#include "ke/binfo.h"
#include "ke/spinlock.h"
#include <ke/ke.h>
#include <ke/thread.h>








boolean KeInitSystem(BootInfo* info){
    boolean result = KeInitializeThreadComponent();
    if(result == FALSE) return FALSE;
    result = KeInitializeSpinlockComponent();
    if(result == FALSE) return FALSE;
    KeStoreBootInformationAddress(info);
    // do more initalization of the kernel
    return TRUE;
}