#include "arch/includes/pmm.h"
#include "arch/includes/vmm.h"
#include "arch/includes/setupar.h"
#include "bootinfo.h"
#include "graphics.h"
#include "qol.h"
#include "term.h"





void KernSystemStartup(BootInfo* info){
    // boring bootinfo setup stuff
    GraphicsData data = {
        info->graphicsData.init,
        info->graphicsData.framebufferBase,
        info->graphicsData.framebufferSize,
        info->graphicsData.width,
        info->graphicsData.height,
        info->graphicsData.pixelsPerScanLine
    };
    GraphicsSetup(&data);
    TermClear();
    TermPrint(TERM_STATUS_PASS, "Entered kernel");
    TermPrint(TERM_STATUS_PASS, "Physical kernel address is 0x%x", info->kernelLocPhys);
    // a long list of stuff to setup...
    ArSetupArchitecture();
    TermPrint(TERM_STATUS_PASS, "we have set up architecture specific structures");
    boolean result = MmInitPhysicalMemoryManager(info->memMap);
    if(result == FALSE) QolPanic("Couldn't Setup Physical Memory Manager");
    TermPrint(TERM_STATUS_PASS, "we have set up the physical memory manager");
    result = MmTestPhysicalMemoryManager();
    if(result == FALSE) QolPanic("Physical Memory Manager failed tests");
    TermPrint(TERM_STATUS_PASS, "Physical memory manager has passed all tests");
    result = MmInitVirtualMemoryManager(info);
    if(result == FALSE) QolPanic("Couldn't Setup Virtual Memory Manager");
    TermPrint(TERM_STATUS_PASS, "we have set up the virtual memory manager");
    TermPrint(TERM_STATUS_PASS, "Initial Kernel Setup Complete!");
    while(1){continue;}
}