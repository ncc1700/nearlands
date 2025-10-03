#include "../../includes/smp.h"
#include "../../../limine.h"
#include "../../../core/tools/debugtools.h"

#include "../../includes/halinit.h"
#include "../../includes/mem.h"
#include "../../includes/spinlock.h"

#include "../../includes/misc.h"
#include <stdatomic.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_smp_request smp_request = {
    LIMINE_SMP_REQUEST,
    0
};

static atomic_flag lock = ATOMIC_FLAG_INIT;
void print_status(struct limine_smp_info *smp_info){
   
}

void cpu_entry(struct limine_smp_info *smp_info){
    sl_acquire(&lock);
    setup_hal();
    DKPRINTTEXTANDDECWITHSPACE(__FILE__, __LINE__);
    DKPRINTTEXTANDDECWITHSPACE(" CPU", smp_info->processor_id);
    DKPRINTLN(" is alive, halting...");
    sl_release(&lock);
    while(1){__asm__ __volatile__("hlt"); continue;}
}

extern void lloop();

uint8_t setup_smp(){
    if(smp_request.response == NULL){
        return 1;
    }
    if(smp_request.response->cpu_count <= 1){
        DKPRINTTEXTANDDECWITHSPACE(__FILE__, __LINE__);
        DKPRINTLN(" CPU only has one core, returning....");
        return 0;
    }
    DKPRINTTEXTANDDECWITHSPACE(__FILE__, __LINE__);
    DKPRINT(" CPU amount is ");
    DKPRINTDECLN(smp_request.response->cpu_count);
    // for(int i = 1; i < smp_request.response->cpu_count; i++){
    //     DKPRINTLN("p");
    //     smp_request.response->cpus[i]->goto_address = cpu_entry;
    // }
    return 0;
}



