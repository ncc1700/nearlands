#ifndef SPINLOCK_H_INCLUDED
#define SPINLOCK_H_INCLUDED








#include <stdint.h>
#include <stdatomic.h>




void sl_acquire(atomic_flag* lock);
void sl_release(atomic_flag* lock);





#endif