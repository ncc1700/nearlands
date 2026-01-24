#ifndef IDT_H_INCLUDED
#define IDT_H_INCLUDED








#include <stdint.h>




void Amd64SetupIDT();
void Amd64LoadIDT(void* idtr);
void Amd64DivideByZeroIntEntry();
void Amd64DebugExceptionIntEntry();
void Amd64NMIIntEntry();
void Amd64BreakpointIntEntry();
void Amd64OverflowIntEntry();
void Amd64BoundRangeEntry();
void Amd64InvalidOpcodeEntry();
void Amd64NoMathProcEntry();
void Amd64DoubleFaultEntry();
void Amd64InvalidTSSEntry();
void Amd64SegmentNotPresentEntry();
void Amd64StackSegmentFaultEntry();
void Amd64GeneralProtectionEntry();
void Amd64PageFaultEntry();
void Amd64FloatingPointErrorEntry();
void Amd64AlignmentCheckEntry();
void Amd64MachineCheckEntry();
void Amd64SIMDFloatingPointErrorEntry();
void Amd64VirtualizationErrorEntry();
void Amd64ControlProtectionExceptionEntry();





#endif