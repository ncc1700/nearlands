#include "ke/panic.h"
#include "mm/includes/pmm.h"
#include "mm/includes/vmm.h"
#include "mm/includes/heap.h"

#include "ar/includes/setupar.h"
#include "bootinfo.h"
#include "ecs/components/threadcmp.h"
#include "ecs/ecs.h"
#include "ke/graphics.h"
#include "ke/splash.h"
#include "ke/term.h"


u64 poo(void* poo){
    KeTermPrint(TERM_STATUS_PASS, "Entered thread!");
    return 69;
}


void KeSystemStartup(BootInfo* info){
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
    KeGraphicsSetup(&data);
    KeBeginSplashScreen(10);
    KeTermClear();
    KeGraphicsDrawRect(0, 0, KeGraphicsReturnData()->width, 42, 0xFA8128);
    KeGraphicsDrawString("NEARLANDS", 10, 10, 2, 0x000000);

    KeTermMoveDown(4);
    KeTermPrint(TERM_STATUS_UNKNOWN, "--------------------PHASE 0--------------------");
    KeAddToBootScreen();
    KeTermPrint(TERM_STATUS_PASS, "Physical kernel address is 0x%x", info->kernelLocPhys);
    // a long list of stuff to setup...
    ArSetupArchitecture();
    KeTermPrint(TERM_STATUS_PASS, "we have set up architecture specific structures");
    KeAddToBootScreen();
    boolean result = MmInitPhysicalMemoryManager(info->memMap);
    if(result == FALSE) KePanic("Couldn't Setup Physical Memory Manager");
    KeTermPrint(TERM_STATUS_PASS, "we have set up the physical memory manager");
    KeAddToBootScreen();
    result = MmTestPhysicalMemoryManager();
    if(result == FALSE) KePanic("Physical Memory Manager failed tests");
    KeTermPrint(TERM_STATUS_PASS, "Physical memory manager has passed all tests");
    KeAddToBootScreen();

    result = MmInitVirtualMemoryManager(info);
    if(result == FALSE) KePanic("Couldn't Setup Virtual Memory Manager");
    KeTermPrint(TERM_STATUS_PASS, "we have set up the virtual memory manager");
    KeAddToBootScreen();
    //KeTermPrint(TERM_STATUS_UNKNOWN, "----------------FINISHED PHASE 0----------------");
    // style stuff =D
    //KeTermPrint(TERM_STATUS_UNKNOWN, " ");
    //KeTermPrint(TERM_STATUS_UNKNOWN, " ");

    KeTermPrint(TERM_STATUS_UNKNOWN, "--------------------PHASE 1--------------------");
    u64 heapSize = info->memMap->sizeOfEntireMemory / 23;
    u64 heapSizeInPages = (heapSize + (PAGE_SIZE - 1)) / PAGE_SIZE;
    KeTermPrint(TERM_STATUS_PASS, "heap size is %d", heapSizeInPages);
    result = MmInitHeapAllocator(heapSizeInPages);
    if(result == FALSE) KePanic("Couldn't Setup Heap Allocator");
    KeTermPrint(TERM_STATUS_PASS, "we have set up the heap allocator");
    //KeTermPrint(TERM_STATUS_INFO, "Testing ECS System...");

    // ThreadComponent tComp;
    // tComp.SysThreadEntry = poo;

    // ComponentTypes comp[1];    
    // comp[0] = COMP_THREAD;


    // u64 i = 1;
    // while(1){
    //     Handle entity = EcsCreateEntity(comp, 1);
    //     KeTermPrint(TERM_STATUS_INFO, "entity %d created", i);
    //     i++;
    // }
    KeTermPrint(TERM_STATUS_ERROR, "WORK IN PROGRESS - come back later!");
    while(1){continue;}
}