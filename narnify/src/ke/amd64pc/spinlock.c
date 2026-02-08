#include "ecs/ecs.h"
#include "mm/alloc.h"
#include "nrstatus.h"
#include <ke/spinlock.h>
#include <stdatomic.h>


// thanks to https://wiki.osdev.org/Spinlock for the help!

static u16 spinlockCompIndex = 0;

NearStatus KeInitializeSpinlockComponent(){
    u16 index = EcsCreateNewComponent(sizeof(SpinLock));
    if(index == 0) return STATUS_CANT_CREATE_ECS_COMP;
    spinlockCompIndex = index;
    return STATUS_SUCCESS;
}

u16 KeReturnSpinlockCompIndex(){
    return spinlockCompIndex;
}

SpinLock KeCreateSpinLock(){
    SpinLock lock = {ATOMIC_FLAG_INIT};
    return lock;
}

SpinLock* KeCreateSpinLockHeap(){
    SpinLock* lock = MmAllocateGeneralMemory(sizeof(SpinLock));
    if(lock == NULL) return NULL;
    lock->lock = (atomic_flag)ATOMIC_FLAG_INIT;
    return lock;
}

boolean KeDeleteSpinLockFromHeap(SpinLock* lock){
    return MmFreeGeneralMemory(lock, sizeof(SpinLock));
}

void KeAcquireSpinLock(SpinLock* spinLock){
    while(atomic_flag_test_and_set_explicit(&spinLock->lock, memory_order_acquire)){
        asm volatile("pause");
    }
}

void KeReleaseSpinLock(SpinLock* spinLock){
    atomic_flag_clear_explicit(&spinLock->lock, memory_order_release);
}