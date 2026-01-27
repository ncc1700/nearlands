#include "idt.h"
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
    KePanic(QSTR("UNKNOWN HARDWARE INTERRUPT"));
    while(1){continue;}
}

void IntUnknownSoftware(){
    KePanic(QSTR("UNKNOWN SOFTWARE INTERRUPT"));
    while(1){continue;}
}

void IntDivideByZero(){
    KePanic(QSTR("Divide By Zero Interrupt"));
    //while(1){continue;}
}

void IntDebugException(){
    KePanic(QSTR("Debug Exception"));
}

void IntNMIInterrupt(){
    KePanic(QSTR("NMI Interrupt"));
}

void IntBreakpointInterrupt(){
    KePanic(QSTR("A breakpoint has been reached\n"));
}

void IntOverflowInterrupt(){
    KePanic(QSTR("Overflow\n"));
}

void IntBoundRangeInterrupt(){
    KePanic(QSTR("BOUND Range has been exceeded\n"));
}

void IntInvalidOpcodeInterrupt(){
    KePanic(QSTR("an invalid opcode has been reached\n"));
}

void IntNoMathProcInterrupt(){
    KePanic(QSTR("no math coprocessor has been fouud\n"));
}

void IntDoubleFaultInterrupt(){
    KePanic(QSTR("Double Fault\n"));
}

void IntInvalidTSSInterrupt(){
    KePanic(QSTR("Invalid TSS\n"));
}

void IntSegmentNotPresentInterrupt(){
    KePanic(QSTR("Segment Not Present\n"));
}

void IntStackSegmentFaultInterrupt(){
    KePanic(QSTR("Stack Segment Fault\n"));
}

void IntGeneralProtectionInterrupt(){
    KePanic(QSTR("General Protection Error\n"));
}

void IntPageFaultInterrupt(){
    KePanic(QSTR("A page fault has occured\n"));
}


void IntFloatingPointErrorInterrupt(){
    KePanic(QSTR("floating point error\n"));
}

void IntAlignmentCheckInterrupt(){
    KePanic(QSTR("invalid alignment\n"));
}

void IntMachineCheckInterrupt(){
    KePanic(QSTR("machine check\n"));
}

void IntSIMDFloatingPointErrorInterrupt(){
    KePanic(QSTR("SIMD Floating Point Error\n"));
}

void IntVirtualizationErrorInterrupt(){
    KePanic(QSTR("Virtualization Error\n"));
}

void IntControlProtectionExceptionInterrupt(){
    KePanic(QSTR("Control Protection Exception\n"));
}

static inline void IntCreateInterrupt(uint8_t i, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t typeattributes){
    uint64_t handleraddr = (uint64_t)handler;
    desc[i].offsetone = handleraddr & 0xFFFF;
    desc[i].selector = selector;
    desc[i].ist = ist & 0x07;
    desc[i].typeattributes = typeattributes;
    desc[i].offsettwo = (handleraddr >> 16) & 0xFFFF;
    desc[i].offsetthree = (handleraddr >> 32) & 0xFFFFFFFF;
    desc[i].zero = 0;
}

void Amd64SetupIDT(){
    for(int i = 32; i < 255; i++){
        IntCreateInterrupt(i, IntUnknownSoftware, 0x08, 0, 0x8E);
    }
    IntCreateInterrupt(0, Amd64DivideByZeroIntEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(1, Amd64DebugExceptionIntEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(2, Amd64NMIIntEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(3, Amd64BreakpointIntEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(4, Amd64OverflowIntEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(5, Amd64BoundRangeEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(6, Amd64InvalidOpcodeEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(7, Amd64NoMathProcEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(8, Amd64DoubleFaultEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(10, Amd64InvalidTSSEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(11, Amd64SegmentNotPresentEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(12, Amd64StackSegmentFaultEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(13, Amd64GeneralProtectionEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(14, Amd64PageFaultEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(16, Amd64FloatingPointErrorEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(17, Amd64AlignmentCheckEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(18, Amd64MachineCheckEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(19, Amd64SIMDFloatingPointErrorEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(20, Amd64VirtualizationErrorEntry, 0x08, 0, 0x8E);
    IntCreateInterrupt(21, Amd64ControlProtectionExceptionEntry, 0x08, 0, 0x8E);

    idt.base = (uint64_t)desc;
    idt.limit = (sizeof(IntDesc) * IDT_SIZE) - 1;
    Amd64LoadIDT(&idt);
}
