#include "../includes/spinlock.h"
#include <stdatomic.h>



// thanks to https://wiki.osdev.org/Spinlock for the help!



void sl_acquire(atomic_flag* lock){
    while(atomic_flag_test_and_set_explicit(lock, memory_order_acquire)){
        //__asm__ __volatile__("pause");
    }
}

void sl_release(atomic_flag* lock)
{
    atomic_flag_clear_explicit(lock, memory_order_release);
}

