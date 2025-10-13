#ifndef SPINLOCK_H_INCLUDED
#define SPINLOCK_H_INCLUDED








#include <stdint.h>
#include <stdatomic.h>




void sl_acquire(volatile atomic_flag* lock);
void sl_release(volatile atomic_flag* lock);





#endif