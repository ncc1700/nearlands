#include "idt.h"
#include "../../../term.h"
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

extern void Amd64LoadIDT(void* idtr);

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

void IntUnknownHardware(){
    TermPrint(TERM_STATUS_ERROR, "UNKNOWN HARDWARE INTERRUPT");
    while(1){continue;}
}

void IntUnknownSoftware(){
    TermPrint(TERM_STATUS_ERROR, "UNKNOWN SOFTWARE INTERRUPT");
    while(1){continue;}
}


void Amd64SetupIDT(){
    for(int i = 0; i < 31; i++){
        IntCreateInterrupt(i, IntUnknownHardware, 0x08, 0, 0x8E);
    }
     for(int i = 32; i < 255; i++){
        IntCreateInterrupt(i, IntUnknownSoftware, 0x08, 0, 0x8E);
    }
    idt.base = (uint64_t)desc;
    idt.limit = (sizeof(IntDesc) * IDT_SIZE) - 1;
    Amd64LoadIDT(&idt);
}
