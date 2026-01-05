section .text


global Amd64LoadIDT
global Amd64DivideByZeroIntEntry
global Amd64DebugExceptionIntEntry
global Amd64NMIIntEntry
global Amd64BreakpointIntEntry
global Amd64OverflowIntEntry
global Amd64BoundRangeEntry
global Amd64InvalidOpcodeEntry
global Amd64NoMathProcEntry
global Amd64DoubleFaultEntry
global Amd64InvalidTSSEntry
global Amd64SegmentNotPresentEntry
global Amd64GeneralProtectionEntry
global Amd64StackSegmentFaultEntry
global Amd64PageFaultEntry
global Amd64FloatingPointErrorEntry
global Amd64AlignmentCheckEntry
global Amd64MachineCheckEntry
global Amd64SIMDFloatingPointErrorEntry
global Amd64VirtualizationErrorEntry
global Amd64ControlProtectionExceptionEntry



extern IntDivideByZero
extern IntDebugException
extern IntNMIInterrupt
extern IntBreakpointInterrupt
extern IntOverflowInterrupt
extern IntBoundRangeInterrupt
extern IntInvalidOpcodeInterrupt
extern IntNoMathProcInterrupt
extern IntDoubleFaultInterrupt
extern IntInvalidTSSInterrupt
extern IntSegmentNotPresentInterrupt
extern IntStackSegmentFaultInterrupt
extern IntGeneralProtectionInterrupt
extern IntPageFaultInterrupt
extern IntFloatingPointErrorInterrupt
extern IntAlignmentCheckInterrupt
extern IntMachineCheckInterrupt
extern IntSIMDFloatingPointErrorInterrupt
extern IntVirtualizationErrorInterrupt
extern IntControlProtectionExceptionInterrupt


Amd64LoadIDT:
    push rbp
    mov rbp, rsp
    lidt [rcx]
    mov rsp, rbp
    pop rbp
    ret


%macro PUSH_REGS 0:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    cld
%endmacro

%macro POP_REGS 0:
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11 
    pop r10 
    pop r9
    pop r8
    pop rdi 
    pop rsi 
    pop rdx
    pop rcx 
    pop rbx 
    pop rax
%endmacro

Amd64DivideByZeroIntEntry:
    PUSH_REGS
    call IntDivideByZero
    POP_REGS
    iret


Amd64DebugExceptionIntEntry:
    PUSH_REGS
    call IntDebugException
    POP_REGS
    iret

Amd64NMIIntEntry:
    PUSH_REGS
    call IntNMIInterrupt
    POP_REGS
    iret

Amd64BreakpointIntEntry:
    PUSH_REGS
    call IntBreakpointInterrupt
    POP_REGS
    iret

Amd64OverflowIntEntry:
    PUSH_REGS
    call IntOverflowInterrupt
    POP_REGS
    iret

Amd64BoundRangeEntry:
    PUSH_REGS
    call IntBoundRangeInterrupt
    POP_REGS
    iret


Amd64InvalidOpcodeEntry:
    PUSH_REGS
    call IntInvalidOpcodeInterrupt
    POP_REGS
    iret

Amd64NoMathProcEntry:
    PUSH_REGS
    call IntNoMathProcInterrupt
    POP_REGS
    iret

Amd64DoubleFaultEntry:
    PUSH_REGS
    call IntDoubleFaultInterrupt
    POP_REGS
    iret

Amd64InvalidTSSEntry:
    PUSH_REGS
    call IntInvalidTSSInterrupt
    POP_REGS
    iret

Amd64SegmentNotPresentEntry:
    PUSH_REGS
    call IntSegmentNotPresentInterrupt
    POP_REGS
    iret

Amd64StackSegmentFaultEntry:
    PUSH_REGS
    call IntStackSegmentFaultInterrupt
    POP_REGS
    iret

Amd64GeneralProtectionEntry:
    PUSH_REGS
    call IntGeneralProtectionInterrupt
    POP_REGS
    iret

Amd64PageFaultEntry:
    PUSH_REGS
    call IntPageFaultInterrupt
    POP_REGS
    iret

Amd64FloatingPointErrorEntry:
    PUSH_REGS
    call IntFloatingPointErrorInterrupt
    POP_REGS
    iret

Amd64AlignmentCheckEntry:
    PUSH_REGS
    call IntAlignmentCheckInterrupt
    POP_REGS
    iret

Amd64MachineCheckEntry:
    PUSH_REGS
    call IntMachineCheckInterrupt
    POP_REGS
    iret

Amd64SIMDFloatingPointErrorEntry:
    PUSH_REGS
    call IntSIMDFloatingPointErrorInterrupt
    POP_REGS
    iret

Amd64VirtualizationErrorEntry:
    PUSH_REGS
    call IntVirtualizationErrorInterrupt
    POP_REGS
    iret

Amd64ControlProtectionExceptionEntry:
    PUSH_REGS
    call IntControlProtectionExceptionInterrupt
    POP_REGS
    iret


