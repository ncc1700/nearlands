#include <ke/thread.h>
#include <ke/term.h>
#include <ecs/ecs.h>


static u16 ktCIndex = 0;
static u16 utCIndex = 0;

boolean KeInitializeThreadComponent(){
    ktCIndex = EcsCreateNewComponent(sizeof(KernThreadComponent));
    if(ktCIndex == 0) return FALSE;
    utCIndex = EcsCreateNewComponent(sizeof(UserThreadComponent));
    if(utCIndex == 0) return FALSE;
    return TRUE;
}

u16 KeReturnKtCompIndex(){
    return ktCIndex;
}

u16 KeReturnUtCompIndex(){
    return utCIndex;
}

void KeCreateThread(){

}

