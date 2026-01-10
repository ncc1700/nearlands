#include "arch/includes/pmm.h"
#include "arch/includes/vmm.h"
#include "arch/includes/heap.h"

#include "arch/includes/setupar.h"
#include "bootinfo.h"
#include "graphics.h"
#include "qol.h"
#include "splash.h"
#include "term.h"





void KernSystemStartup(BootInfo* info){
    // boring bootinfo setup stuff
    GraphicsContextOwners owners;
    if(info->typeOfBoot == 1) owners = OWNER_DEBUG_TERM;
    else owners = OWNER_SPLASH_SCREEN;
    GraphicsData data = {
        info->graphicsData.init,
        info->graphicsData.framebufferBase,
        info->graphicsData.framebufferSize,
        info->graphicsData.width,
        info->graphicsData.height,
        info->graphicsData.pixelsPerScanLine,
        owners
    };
    GraphicsSetup(&data);
    KernBeginSplashScreen(10);
    TermClear();
    TermPrint(TERM_STATUS_PASS, "Entered kernel");
    TermPrint(TERM_STATUS_PASS, "Physical kernel address is 0x%x", info->kernelLocPhys);
    // style stuff =D
    TermPrint(TERM_STATUS_UNKNOWN, " ");
    TermPrint(TERM_STATUS_UNKNOWN, " ");
    TermPrint(TERM_STATUS_UNKNOWN, "--------------------PHASE 0--------------------");
    KernAddToBootScreen();
    // a long list of stuff to setup...
    ArSetupArchitecture();
    TermPrint(TERM_STATUS_PASS, "we have set up architecture specific structures");
    KernAddToBootScreen();
    boolean result = MmInitPhysicalMemoryManager(info->memMap);
    if(result == FALSE) QolPanic("Couldn't Setup Physical Memory Manager");
    TermPrint(TERM_STATUS_PASS, "we have set up the physical memory manager");
    KernAddToBootScreen();
    result = MmTestPhysicalMemoryManager();
    if(result == FALSE) QolPanic("Physical Memory Manager failed tests");
    TermPrint(TERM_STATUS_PASS, "Physical memory manager has passed all tests");
    KernAddToBootScreen();
    result = MmInitVirtualMemoryManager(info);
    if(result == FALSE) QolPanic("Couldn't Setup Virtual Memory Manager");
    TermPrint(TERM_STATUS_PASS, "we have set up the virtual memory manager");
    KernAddToBootScreen();
    TermPrint(TERM_STATUS_UNKNOWN, "----------------FINISHED PHASE 0----------------");
    // style stuff =D
    TermPrint(TERM_STATUS_UNKNOWN, " ");
    TermPrint(TERM_STATUS_UNKNOWN, " ");

    TermPrint(TERM_STATUS_UNKNOWN, "--------------------PHASE 1--------------------");

    result = MmInitHeapAllocator(1);
    if(result == FALSE) QolPanic("Couldn't Setup Kernel Heap");
    TermPrint(TERM_STATUS_PASS, "we have set up the kernel heap");
    

    TermPrint(TERM_STATUS_ERROR, "WORK IN PROGRESS - come back later!");
    while(1){continue;}
}