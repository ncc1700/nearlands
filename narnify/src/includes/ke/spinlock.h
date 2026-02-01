#ifndef SPINLOCK_H_INCLUDED
#define SPINLOCK_H_INCLUDED









#include <types.h>
#include <stdatomic.h>

// we use C atomics, might change it later

typedef struct _SpinLock {
    atomic_flag lock;
} SpinLock;

SpinLock KeCreateSpinLock();
SpinLock* KeCreateSpinLockHeap();
boolean KeDeleteSpinLockFromHeap(SpinLock* lock);
void KeAcquireSpinLock(SpinLock* spinLock);
void KeReleaseSpinLock(SpinLock* spinLock);


#endif