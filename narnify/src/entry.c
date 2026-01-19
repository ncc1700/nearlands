#include "arch/includes/pmm.h"
#include "arch/includes/vmm.h"
#include "arch/includes/heap.h"

#include "arch/includes/setupar.h"
#include "bootinfo.h"
#include "executive/ecs/components/stackcmp.h"
#include "executive/ecs/components/threadcmp.h"
#include "executive/ecs/ecs.h"
#include "graphics.h"
#include "qol.h"
#include "splash.h"
#include "term.h"


u64 poo(u64* poo){
    TermPrint(TERM_STATUS_PASS, "Entered thread!");
}


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
    TermPrint(TERM_STATUS_INFO, "Testing ECS System...");

    ThreadComponent tComp = {NULL};
    StackComponent sComp = {NULL};

    Component comp[1];
    Component comp2[2];
    comp[0].type = COMP_THREAD;
    comp[0].componentData = (void*)&tComp;
    comp2[0].type = COMP_THREAD;
    comp2[0].componentData = (void*)&tComp;
    comp2[1].type = COMP_STACK;
    comp2[1].componentData = (void*)&sComp;

    u32 archeType = 0;
    u32 entity = 0;

    ref entity1 = EcsCreateEntity(comp, 1);
    EcsDecodeReference(entity1, &archeType, &entity);
    TermPrint(TERM_STATUS_PASS, "Created Entity (a: %d, e: %d) (0x%x)", archeType, entity, entity1);
    ref entity2 = EcsCreateEntity(comp, 1);
    EcsDecodeReference(entity2, &archeType, &entity);
        TermPrint(TERM_STATUS_PASS, "Created Entity (a: %d, e: %d) (0x%x)", archeType, entity, entity2);
    ref entity3 = EcsCreateEntity(comp2, 1);
    EcsDecodeReference(entity3, &archeType, &entity);
        TermPrint(TERM_STATUS_PASS, "Created Entity (a: %d, e: %d) (0x%x)", archeType, entity, entity3);


    ref entity4 = EcsCreateEntity(comp2, 2);
    Component* component = EcsGetComponent(entity4, COMP_THREAD);
    if(component != NULL){
        TermPrint(TERM_STATUS_PASS, "Found component in Entity");
        ThreadComponent* threadComp = (ThreadComponent*)component->componentData;
        // threadComp->SysThreadEntry(NULL); -- wont work for now, make it fixed
    } else TermPrint(TERM_STATUS_PASS, "Couldn't find component");

    TermPrint(TERM_STATUS_ERROR, "WORK IN PROGRESS - come back later!");
    while(1){continue;}
}