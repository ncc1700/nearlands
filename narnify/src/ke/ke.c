
#include "ke.h"
#include "thread.h"








boolean KeInitSystem(){
    boolean result = KeInitalizeThreadComponent();
    if(result == FALSE) return FALSE;
    // do more initalization of the kernel
    return TRUE;
}