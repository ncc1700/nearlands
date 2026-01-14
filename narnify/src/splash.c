#include "splash.h"
#include "graphics.h"



static const char* quotes[] = {
    "Live Laugh Love",
    "Never give up",
    "You are valid, whatever you are",
    "Never listen to dividers",
    "I like the green bay packers",
    "Never loose",
    "DrDonut_Meow.png",
    "DrCoconut",
    "DrD",
    "Check out ReactOS!",
    "Check out Haiku!",
    "Check out Windows!",
    "Live your life!",
    "NtAllocateVirtualMemory"
};

static u64 dotX = 10;


void KernAddToBootScreen(){
    if(GraphicsReturnData()->owners != OWNER_SPLASH_SCREEN) return;
    GraphicsDrawChar('/', dotX, 40, 3, 0x00ffff);
    dotX += 10;
}

void KernBeginSplashScreen(u64 dashes){
    if(GraphicsReturnData()->owners != OWNER_SPLASH_SCREEN) return;
    GraphicsClear(0x000000);
    u64 midWidth = GraphicsReturnData()->width / 2;
    u64 midHeight = GraphicsReturnData()->height / 2;
    GraphicsDrawString("Starting Nearlands", 10, 10, 2, 0xffffff);
    for(u64 i = 10; i < 10 + (dashes * 10); i+=10){
        GraphicsDrawChar('/', i, 40, 3, 0xffffff);
    }
    //GraphicsDrawString("//////", 10, 40, 3, 0xffffff);
    
    //GraphicsDrawString(quotes[6], 10, logoOffset.y + 10, 1, 0xff00ff);
}