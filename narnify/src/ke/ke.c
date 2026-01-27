
#include <ke/ke.h>
#include <ke/thread.h>








boolean KeInitSystem(){
    boolean result = KeInitializeThreadComponent();
    if(result == FALSE) return FALSE;
    // do more initalization of the kernel
    return TRUE;
}