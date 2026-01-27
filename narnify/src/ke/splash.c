#include <ke/splash.h>
#include <ke/graphics.h>



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


void KeAddToBootScreen(){
    if(KeGraphicsReturnData()->owners != OWNER_SPLASH_SCREEN) return;
    KeGraphicsDrawChar('/', dotX, 40, 3, 0x00ffff);
    dotX += 10;
}

void KeBeginSplashScreen(u64 dashes){
    if(KeGraphicsReturnData()->owners != OWNER_SPLASH_SCREEN) return;
    KeGraphicsClear(0x000000);
    u64 midWidth = KeGraphicsReturnData()->width / 2;
    u64 midHeight = KeGraphicsReturnData()->height / 2;
    KeGraphicsDrawString(QSTR("Starting Nearlands"), 10, 10, 2, 0xffffff);
    for(u64 i = 10; i < 10 + (dashes * 10); i+=10){
        KeGraphicsDrawChar('/', i, 40, 3, 0xffffff);
    }
    //GraphicsDrawString("//////", 10, 40, 3, 0xffffff);
    
    //GraphicsDrawString(quotes[6], 10, logoOffset.y + 10, 1, 0xff00ff);
}