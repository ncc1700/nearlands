#include "apic.h"
#include "ke/term.h"
#include "mm/vmm.h"
#include "nrstatus.h"
#include "uacpi/status.h"
#include "uacpi/tables.h"



static uacpi_table apicTable = {0};
static u64 lapicBase = 0;

#define APIC_LVT_INT_MASKED (1 << 16)
#define APIC_LVT_PERIODIC (1 << 17)

extern u64 ArRdmsr(u64 id);
extern u64 ArWrmsr(u64 id, u64 high, u64 low);
extern void ArIntCreateInterrupt(uint8_t i, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t typeattributes);
extern void ArIntLoadInterrupt();
typedef enum _APICOffsets {
    APIC_LAPIC_ID = 0x20, 
    APIC_LAPIC_VER = 0x30,
    APIC_TASK_PRIORITY = 0x80,
    APIC_ARBITRATION_PRIORITY = 0x90,
    APIC_PROCESSOR_PRIORITY = 0xA0,
    APIC_EOI = 0xB0,
    APIC_REMOTE_READ = 0xC0,
    APIC_LOGICAL_DEST = 0xD0,
    APIC_DEST_FORMAT = 0xE0,
    APIC_SPURIOUS_INT_VEC = 0xF0,
    APIC_IN_SERVICE_BEG = 0x100,
    APIC_IN_SERVICE_END = 0x170,
    APIC_TRIGGER_MODE_BEG = 0x180,
    APIC_TRIGGER_MODE_END = 0x1F0,
    APIC_INTERRUPT_REQ_BEG = 0x200,
    APIC_INTERRUPT_REQ_END = 0x270,
    APIC_ERROR_STATUS = 0x280,
    APIC_LVT_CORRECT_MACHINE_CHECK_INT = 0x2F0,
    APIC_INTERRUPT_COMMAND_BEG = 0x300,
    APIC_INTERRUPT_COMMAND_END = 0x310,
    APIC_LVT_TIMER = 0x320,
    APIC_LVT_THERMAL_SENSOR = 0x330,
    APIC_LVT_PERF_MON_COUNT = 0x340,
    APIC_LVT_LINT0 = 0x350,
    APIC_LVT_LINT1 = 0x360,
    APIC_LVT_ERROR = 0x370,
    APIC_TIMER_INITIAL_COUNT = 0x380,
    APIC_TIMER_CURRENT_COUNT = 0x390,
    APIC_TIMER_DIVIDE_CONFIG = 0x3E0,
    LAPIC_MAX = 0x3E0
} APICOffsets;

void IntApicTimer(){
    KeTermPrint(TERM_STATUS_INFO, QSTR("h"));
    KiApicWrite(APIC_EOI, 0);
    //while(1){continue;}
}

static inline void ApicCalibrateAndStartTimer(){
    KiApicWrite(APIC_TIMER_DIVIDE_CONFIG, 0x3);
    // KiApicWrite(APIC_TIMER_INITIAL_COUNT, -1);
    
    //KiApicWrite(APIC_LVT_TIMER, APIC_LVT_INT_MASKED);
    //u32 ticks = KiApicRead(APIC_TIMER_CURRENT_COUNT);
    KiApicWrite(APIC_LVT_TIMER, 
                32 | APIC_LVT_PERIODIC);
    KiApicWrite(APIC_TIMER_DIVIDE_CONFIG, 0x3);
    KiApicWrite(APIC_TIMER_INITIAL_COUNT, 10000);

    ArIntCreateInterrupt(32, IntApicTimer, 0x08, 0, 0x8E);
    ArIntLoadInterrupt();
}

NearStatus KiInitApic(){
    uacpi_status stat = uacpi_table_find_by_signature("APIC", &apicTable);
    if(stat != UACPI_STATUS_OK){
        return STATUS_NOT_FOUND;
    }
    __asm__ __volatile__("cli");
    u64 apicBase = ArRdmsr(0x1B);
    lapicBase = apicBase & 0xFFFFF000;
    apicBase |= (1ULL << 11);
    ArWrmsr(0x1B, apicBase >> 32, apicBase & 0xFFFFFFFF);
    apicBase = ArRdmsr(0x1B);
    KiApicWrite(APIC_TASK_PRIORITY, 0);
    KiApicWrite(APIC_SPURIOUS_INT_VEC, 0x100 | 0xFF);
    //ApicCalibrateAndStartTimer();
    //     __asm__ __volatile__("sti"); <- we
    //KeTermPrint(TERM_STATUS_INFO, QSTR("0x%x"), apicTable.virt_addr);
    return STATUS_SUCCESS;
}

u64 KiReturnAPICAddress(){
    return apicTable.virt_addr;
}

u64 KiApicIoRead(u64 ioAddr, u32 reg){
    volatile u32* addr = (volatile u32*)ioAddr;
    addr[0] = (reg & 0xff);
    return addr[4];
}

void KiApicIoWrite(u64 ioAddr, u32 reg, u32 value){
    volatile u32* addr = (volatile u32*)ioAddr;
    addr[0] = (reg & 0xff);
    addr[4] = value;
}

u64 KiApicRead(u64 offset){
    u64 apicAddr = lapicBase;
    volatile u32* addr = (volatile u32*)(apicAddr + offset);
    return *addr;
}

void KiApicWrite(u64 offset, u32 value){
    u64 apicAddr = lapicBase;
    volatile u32* addr = (volatile u32*)(apicAddr + offset);
    *addr = value;;
}

