#include "arch/amd64pc/mm/pmm.h"
#include "arch/amd64pc/mm/vmm.h"
#include "arch/includes/setupar.h"
#include "bootinfo.h"
#include "graphics.h"
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
    TermPrint(TERM_STATUS_PASS, "set up architecture specific structures");
    MmInitPhysicalMemoryManager(info->memMap);
    TermPrint(TERM_STATUS_PASS, "set up the physical memory manager (stub)");
    MmInitVirtualMemoryManager(info->memMap);
    TermPrint(TERM_STATUS_PASS, "set up the virtual memory manager (stub)");

    TermPrint(TERM_STATUS_PASS, "Initial Kernel Setup Complete!");

    *(u64*)0x292028298292892 = 'h'; // should crash
    while(1){continue;}
}