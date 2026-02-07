#include "ar/io.h"
#include "ecs/ecs.h"
#include "ke/binfo.h"
#include "mm/alloc.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "nrstatus.h"
#include "uacpi/status.h"
#include "uacpi/types.h"
#include <ke/spinlock.h>
#include <uacpi/kernel_api.h>
#include <ke/term.h>

// our general allocator is very broken (for freeing)
// currently, we will just directly
// use the page allocator until
// I fix it
#define USE_PAGE_ALLOCATOR

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len){
    // for(u64 i = 0; i < len; i+=0x1000){
    //     if(MmCheckIfPageIsMapped(MmReturnKernelPageTable(), addr + i) == FALSE){
    //         KeTermPrint(TERM_STATUS_INFO, QSTR("[ACPI] uACPI tried to use an unmapped page, mapping..."));
    //         MmMapPage(MmReturnKernelPageTable(), addr + i, addr + i, 
    //                             PG_READ_WRITE);
    //     }
    // }
    return (void*)addr;
}

void uacpi_kernel_unmap(void *addr, uacpi_size len){
    return;
}

void uacpi_kernel_log(uacpi_log_level log, const uacpi_char* str){
    Status stat = TERM_STATUS_UNKNOWN;
    switch(log){
        case UACPI_LOG_TRACE:
            stat = TERM_STATUS_IMPINFO;
            break;
        case UACPI_LOG_DEBUG:
        case UACPI_LOG_INFO:
            stat = TERM_STATUS_INFO;
            break;
        case UACPI_LOG_ERROR:
            stat = TERM_STATUS_ERROR;
            break;
        case UACPI_LOG_WARN:
            stat = TERM_STATUS_WARNING;
            break;
        default:
            stat = TERM_STATUS_UNKNOWN;
            break;
    }
    KeTermPrint(stat, QSTR("[uACPI]: %s"), str);
}

void *uacpi_kernel_alloc(uacpi_size size){
    //KeTermPrint(TERM_STATUS_INFO, QSTR("[ACPI]: allocating memory with %d size"), size);
    #ifdef USE_PAGE_ALLOCATOR
    u64 sizeInPages = (size + (PAGE_SIZE - 1)) / PAGE_SIZE;
    return MmAllocateMultiplePages(sizeInPages);
    #else
    return MmAllocateGeneralMemory(size);
    #endif
    
}

void uacpi_kernel_free(void *mem, uacpi_size size_hint){
    // KeTermPrint(TERM_STATUS_INFO, QSTR("[ACPI]: freeing 0x%x with %d size"), 
    //                             mem, size_hint);
    #ifdef USE_PAGE_ALLOCATOR
    u64 sizeInPages = (size_hint + (PAGE_SIZE - 1)) / PAGE_SIZE;
    boolean result = MmFreeMultiplePages(mem, sizeInPages);
    if(!NR_SUCCESS(result)){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: failed freeing 0x%x with %d pages"), 
                                mem, sizeInPages);
    }
    #else
    boolean result = MmFreeGeneralMemory(mem);
    if(!NR_SUCCESS(result)){
        KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: failed freeing 0x%x with %d size"), 
                                mem, size_hint);
    }
    #endif
}

uacpi_u64 uacpi_kernel_get_nanoseconds_since_boot(void){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}

void uacpi_kernel_stall(uacpi_u8 usec){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

void uacpi_kernel_sleep(uacpi_u64 msec){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

uacpi_handle uacpi_kernel_create_mutex(void){
    return uacpi_kernel_create_spinlock();;
}

void uacpi_kernel_free_mutex(uacpi_handle mutexHandle){
    uacpi_kernel_free_spinlock(mutexHandle);
}

uacpi_thread_id uacpi_kernel_get_thread_id(void){
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}

uacpi_status uacpi_kernel_acquire_mutex(uacpi_handle mutexHandle, uacpi_u16 millis){
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    KeAcquireSpinLock(mutexHandle);
    return UACPI_STATUS_OK;
}

void uacpi_kernel_release_mutex(uacpi_handle mutexHandle){
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    KeReleaseSpinLock(mutexHandle);
}

uacpi_bool uacpi_kernel_wait_for_event(uacpi_handle, uacpi_u16){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return FALSE;
}

void uacpi_kernel_signal_event(uacpi_handle){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

void uacpi_kernel_reset_event(uacpi_handle){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

uacpi_status uacpi_kernel_handle_firmware_request(uacpi_firmware_request*){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}

uacpi_status uacpi_kernel_install_interrupt_handler(
    uacpi_u32 irq, uacpi_interrupt_handler, uacpi_handle ctx,
    uacpi_handle *out_irq_handle
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}


uacpi_status uacpi_kernel_uninstall_interrupt_handler(
    uacpi_interrupt_handler, uacpi_handle irq_handle
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}


uacpi_handle uacpi_kernel_create_spinlock(void){
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return KeCreateSpinLockHeap();
}

void uacpi_kernel_free_spinlock(uacpi_handle spinlockHandle){
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__); 
    KeDeleteSpinLockFromHeap(spinlockHandle);
}

uacpi_cpu_flags uacpi_kernel_lock_spinlock(uacpi_handle spinLockHandle){
    KeAcquireSpinLock(spinLockHandle);
    return 0;
}

void uacpi_kernel_unlock_spinlock(uacpi_handle spinLockHandle, uacpi_cpu_flags cpuFlags){
    KeReleaseSpinLock(spinLockHandle);
}


uacpi_status uacpi_kernel_wait_for_work_completion(void){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

uacpi_status uacpi_kernel_io_read8(
    uacpi_handle handle, uacpi_size offset, uacpi_u8 *out_value
){
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    *out_value = ArIoInByte(target);
    return 0;
}
uacpi_status uacpi_kernel_io_read16(
    uacpi_handle handle, uacpi_size offset, uacpi_u16 *out_value
){
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    *out_value = ArIoInWord(target);
    return 0;
}
uacpi_status uacpi_kernel_io_read32(
    uacpi_handle handle, uacpi_size offset, uacpi_u32 *out_value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}

uacpi_status uacpi_kernel_io_write8(
    uacpi_handle handle, uacpi_size offset, uacpi_u8 in_value
){
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    ArIoOutByte(target, in_value);
    return 0;
}
uacpi_status uacpi_kernel_io_write16(
    uacpi_handle handle, uacpi_size offset, uacpi_u16 in_value
){
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    ArIoOutWord(target, in_value);
    return 0;
}
uacpi_status uacpi_kernel_io_write32(
    uacpi_handle, uacpi_size offset, uacpi_u32 in_value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}
uacpi_status uacpi_kernel_pci_read8(
    uacpi_handle device, uacpi_size offset, uacpi_u8 *value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}
uacpi_status uacpi_kernel_pci_read16(
    uacpi_handle device, uacpi_size offset, uacpi_u16 *value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}
uacpi_status uacpi_kernel_pci_read32(
    uacpi_handle device, uacpi_size offset, uacpi_u32 *value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}

uacpi_status uacpi_kernel_pci_write8(
    uacpi_handle device, uacpi_size offset, uacpi_u8 value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}
uacpi_status uacpi_kernel_pci_write16(
    uacpi_handle device, uacpi_size offset, uacpi_u16 value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}
uacpi_status uacpi_kernel_pci_write32(
    uacpi_handle device, uacpi_size offset, uacpi_u32 value
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}

uacpi_status uacpi_kernel_pci_device_open(
    uacpi_pci_address address, uacpi_handle *out_handle
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}
void uacpi_kernel_pci_device_close(uacpi_handle){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

uacpi_status uacpi_kernel_io_map(
    uacpi_io_addr base, uacpi_size len, uacpi_handle *out_handle
){
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    *out_handle = (uacpi_handle)base;
    return 0;
}

void uacpi_kernel_io_unmap(uacpi_handle handle){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address){
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    *out_rsdp_address = KeReturnBootInformation()->rsdp;
    return UACPI_STATUS_OK;
}

uacpi_handle uacpi_kernel_create_event(void){

    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return uacpi_kernel_create_mutex();
}

void uacpi_kernel_free_event(uacpi_handle eventHandle){
    uacpi_kernel_free_mutex(eventHandle);
    //KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
}

uacpi_status uacpi_kernel_schedule_work(
    uacpi_work_type, uacpi_work_handler, uacpi_handle ctx
){
    KeTermPrint(TERM_STATUS_ERROR, QSTR("[ACPI]: %s: UNIMPLEMENTED"), __FUNCTION__);
    return 0;
}