#include "acpi/acpi.h"
#include "ke/binfo.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include <ecs/ecs.h>
#include <ke/ke.h>
#include <ke/panic.h>
#include <ke/thread.h>
#include <mm/mm.h>

#include <ar/ar.h>
#include <bootinfo.h>
#include <ke/graphics.h>
#include <ke/splash.h>
#include <ke/term.h>
#include <qol/qstring.h>
#include <types.h>

void _fltused(){}



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
    KeGraphicsDrawString(QSTR("NEARLANDS"), 
                                            10, 10, 2, 0x000000);

    KeTermMoveDown(4);
    KeTermPrint(TERM_STATUS_UNKNOWN, QSTR("--------------------PHASE 0--------------------"));
    KeAddToBootScreen();
    KeTermPrint(TERM_STATUS_PASS, QSTR("Physical kernel address is 0x%x"), info->kernelLocPhys);
    // a long list of stuff to setup...
    
    KeTermPrint(TERM_STATUS_IMPINFO, QSTR("Calling ArInitSystem"));
    boolean result = ArInitSystem();
    if(result == FALSE) KePanic(QSTR("Couldn't Setup Architecture"));
    else KeTermPrint(TERM_STATUS_PASS, QSTR("ArInitSystem PASS"));

    KeAddToBootScreen();
    KeTermPrint(TERM_STATUS_IMPINFO, QSTR("Calling MmInitSystem"));
    result = MmInitSystem(info);
    if(result == FALSE) KePanic(QSTR("Couldn't Setup Memory Manager"));
    else KeTermPrint(TERM_STATUS_PASS, QSTR("MmInitSystem PASS"));
    KeTermPrint(TERM_STATUS_UNKNOWN, QSTR("--------------------PHASE 1--------------------"));

    KeTermPrint(TERM_STATUS_IMPINFO, QSTR("Calling KeInitSystem"));
    result = KeInitSystem(info);
    if(result == FALSE) KePanic(QSTR("Couldn't Setup Kernel Components"));
    else KeTermPrint(TERM_STATUS_PASS, QSTR("KeInitSystem PASS"));
    

    // KeTermPrint(TERM_STATUS_IMPINFO, QSTR("Calling AcpiInitSystem"));
    // result = AcpiInitSystem();
    // if(result == FALSE) KePanic(QSTR("Couldn't Setup ACPI (via uACPI)"));
    // else KeTermPrint(TERM_STATUS_PASS, QSTR("AcpiInitSystem PASS"));
    
    ComponentTypes types[2] = {KeReturnKtCompIndex(), KeReturnUtCompIndex()};
    int i = 0;
    while(1){
        KeTermPrint(TERM_STATUS_PASS, QSTR("Created Entity %d!"), i);
        EcsCreateEntity(types, 2);
        i++;
    }
    

    KeTermPrint(TERM_STATUS_ERROR, QSTR("WORK IN PROGRESS - come back later!"));

    while(1){continue;}
}