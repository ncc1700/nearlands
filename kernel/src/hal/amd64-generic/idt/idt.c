#include "idt.h"
#include "../../../core/tools/debugtools.h"




#define IDTSIZE 256

typedef struct _intdesc {
   uint16_t offsetone;        
   uint16_t selector;        
   uint8_t  ist;             
   uint8_t  typeattributes; 
   uint16_t offsettwo;        
   uint32_t offsetthree;        
   uint32_t zero;            
} intdesc;

typedef struct _idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr;

static intdesc desc[IDTSIZE];
static idtr idt;

extern void load_idt(void* idtr);

static inline void create_int(uint8_t i, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t typeattributes){
    uint64_t handleraddr = (uint64_t)handler;
    desc[i].offsetone = handleraddr & 0xFFFF;
    desc[i].selector = selector;
    desc[i].ist = ist;
    desc[i].typeattributes = typeattributes;
    desc[i].offsettwo = (handleraddr << 16) & 0xFFFF;
    desc[i].offsetthree = (handleraddr << 32) & 0xFFFF;
    desc[i].zero = 0;
}

void unknown_hardware(){
    DKPRINTLN("UNKNOWN HARDWARE INTERRUPT");
    while(1){continue;}
}

void unknown_software(){
    DKPRINTLN("UNKNOWN SOFTWARE INTERRUPT");
    while(1){continue;}
}


void setup_idt(){
    for(int i = 0; i < 31; i++){
        create_int(i, unknown_hardware, 0x08, 0, 0x8E);
    }
     for(int i = 32; i < 255; i++){
        create_int(i, unknown_software, 0x08, 0, 0x8E);
    }
    idt.base = (uint64_t)desc;
    idt.limit = sizeof(desc) - 1;
    load_idt(&idt);
}