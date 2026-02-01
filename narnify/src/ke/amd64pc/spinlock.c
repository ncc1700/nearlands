#include "mm/heap.h"
#include <ke/spinlock.h>
#include <stdatomic.h>


// thanks to https://wiki.osdev.org/Spinlock for the help!

SpinLock KeCreateSpinLock(){
    SpinLock lock = {ATOMIC_FLAG_INIT};
    return lock;
}

SpinLock* KeCreateSpinLockHeap(){
    SpinLock* lock = MmAllocateFromHeap(sizeof(SpinLock));
    if(lock == NULL) return NULL;
    lock->lock = (atomic_flag)ATOMIC_FLAG_INIT;
    return lock;
}

boolean KeDeleteSpinLockFromHeap(SpinLock* lock){
    return MmFreeFromHeap(lock, sizeof(SpinLock));
}

void KeAcquireSpinLock(SpinLock* spinLock){
    while(atomic_flag_test_and_set_explicit(&spinLock->lock, memory_order_acquire)){
        asm volatile("pause");
    }
}

void KeReleaseSpinLock(SpinLock* spinLock){
    atomic_flag_clear_explicit(&spinLock->lock, memory_order_release);
}