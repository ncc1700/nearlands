#include "idt.h"
#include "ke/term.h"
#include <ke/panic.h>
#define IDT_SIZE 256

typedef struct _IntDesc {
   uint16_t offsetone;
   uint16_t selector;
   uint8_t  ist;
   uint8_t  typeattributes;
   uint16_t offsettwo;
   uint32_t offsetthree;
   uint32_t zero;
} IntDesc;

typedef struct _IdtR {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) IdtR;

static IntDesc desc[IDT_SIZE];
static IdtR idt;

// for now they are very barebones
// will make it a bit more... sane later on

void IntUnknownHardware(){
    KePanicStr(QSTR("UNKNOWN HARDWARE INTERRUPT"));
    while(1){continue;}
}

void IntUnknownSoftware(){
    KePanicStr(QSTR("UNKNOWN SOFTWARE INTERRUPT"));
    while(1){continue;}
}

void IntDivideByZero(){
    KePanicStr(QSTR("Divide By Zero Interrupt"));
    //while(1){continue;}
}

void IntDebugException(){
    KePanicStr(QSTR("Debug Exception"));
}

void IntNMIInterrupt(){
    KePanicStr(QSTR("NMI Interrupt"));
}

void IntBreakpointInterrupt(){
    KePanicStr(QSTR("A breakpoint has been reached\n"));
}

void IntOverflowInterrupt(){
    KePanicStr(QSTR("Overflow\n"));
}

void IntBoundRangeInterrupt(){
    KePanicStr(QSTR("BOUND Range has been exceeded\n"));
}

void IntInvalidOpcodeInterrupt(){
    KePanicStr(QSTR("an invalid opcode has been reached\n"));
}

void IntNoMathProcInterrupt(){
    KePanicStr(QSTR("no math coprocessor has been fouud\n"));
}

void IntDoubleFaultInterrupt(){
    KePanicStr(QSTR("Double Fault\n"));
}

void IntInvalidTSSInterrupt(){
    KePanicStr(QSTR("Invalid TSS\n"));
}

void IntSegmentNotPresentInterrupt(){
    KePanicStr(QSTR("Segment Not Present\n"));
}

void IntStackSegmentFaultInterrupt(){
    KePanicStr(QSTR("Stack Segment Fault\n"));
}

void IntGeneralProtectionInterrupt(){
    KePanicStr(QSTR("General Protection Fault\n"));
}

void IntPageFaultInterrupt(){
    KePanicStr(QSTR("A page fault has occured\n"));
}


void IntFloatingPointErrorInterrupt(){
    KePanicStr(QSTR("floating point error\n"));
}

void IntAlignmentCheckInterrupt(){
    KePanicStr(QSTR("invalid alignment\n"));
}

void IntMachineCheckInterrupt(){
    KePanicStr(QSTR("machine check\n"));
}

void IntSIMDFloatingPointErrorInterrupt(){
    KePanicStr(QSTR("SIMD Floating Point Error\n"));
}

void IntVirtualizationErrorInterrupt(){
    KePanicStr(QSTR("Virtualization Error\n"));
}

void IntControlProtectionExceptionInterrupt(){
    KePanicStr(QSTR("Control Protection Exception\n"));
}

void ArIntCreateInterrupt(uint8_t i, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t typeattributes){
    uint64_t handleraddr = (uint64_t)handler;
    desc[i].offsetone = handleraddr & 0xFFFF;
    desc[i].selector = selector;
    desc[i].ist = ist & 0x07;
    desc[i].typeattributes = typeattributes;
    desc[i].offsettwo = (handleraddr >> 16) & 0xFFFF;
    desc[i].offsetthree = (handleraddr >> 32) & 0xFFFFFFFF;
    desc[i].zero = 0;
}

void ArIntLoadInterrupt(){
    Amd64LoadIDT(&idt);
}

void h(){
    KeTermPrint(TERM_STATUS_INFO, QSTR("tick!"));
}

void Amd64SetupIDT(){
    for(int i = 31; i < 255; i++){
        ArIntCreateInterrupt(i, IntUnknownSoftware, 0x08, 0, 0x8E);
    }
    ArIntCreateInterrupt(0, Amd64DivideByZeroIntEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(1, Amd64DebugExceptionIntEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(2, Amd64NMIIntEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(3, Amd64BreakpointIntEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(4, Amd64OverflowIntEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(5, Amd64BoundRangeEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(6, Amd64InvalidOpcodeEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(7, Amd64NoMathProcEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(8, Amd64DoubleFaultEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(10, Amd64InvalidTSSEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(11, Amd64SegmentNotPresentEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(12, Amd64StackSegmentFaultEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(13, Amd64GeneralProtectionEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(14, Amd64PageFaultEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(16, Amd64FloatingPointErrorEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(17, Amd64AlignmentCheckEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(18, Amd64MachineCheckEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(19, Amd64SIMDFloatingPointErrorEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(20, Amd64VirtualizationErrorEntry, 0x08, 0, 0x8E);
    ArIntCreateInterrupt(21, Amd64ControlProtectionExceptionEntry, 0x08, 0, 0x8E);


    idt.base = (uint64_t)desc;
    idt.limit = (sizeof(IntDesc) * IDT_SIZE) - 1;
    Amd64LoadIDT(&idt);
}
