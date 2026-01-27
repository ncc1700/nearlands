#include <uacpi/kernel_api.h>
#include "../../../ke/term.h"
#include "../../../mm/includes/heap.h"


void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len){
    // UNIMPLEMENTED
    return NULL;
}

void uacpi_kernel_unmap(void *addr, uacpi_size len){
    // UNIMPLEMENTED
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
    KeTermPrint(stat, QSTR(str));
}

void *uacpi_kernel_alloc(uacpi_size size){
    return MmAllocateFromHeap(size);
}

void uacpi_kernel_free(void *mem, uacpi_size size_hint){
    MmFreeFromHeap(mem, size_hint);
}

uacpi_u64 uacpi_kernel_get_nanoseconds_since_boot(void){
    // UNIMPLEMENTED
    return 0;
}

void uacpi_kernel_stall(uacpi_u8 usec){
    // UNIMPLEMENTED
}

void uacpi_kernel_sleep(uacpi_u64 msec){
    // UNIMPLEMENTED
}

uacpi_handle uacpi_kernel_create_mutex(void){
    // UNIMPLEMENTED
    return NULL;
}

void uacpi_kernel_free_mutex(uacpi_handle){
    // UNIMPLEMENTED
}

uacpi_thread_id uacpi_kernel_get_thread_id(void){
    // UNIMPLEMENTED
    return 0;
}

uacpi_status uacpi_kernel_acquire_mutex(uacpi_handle, uacpi_u16){
    // UNIMPLEMENTED
    return 0;
}

void uacpi_kernel_release_mutex(uacpi_handle){
    // UNIMPLEMENTED
}

uacpi_bool uacpi_kernel_wait_for_event(uacpi_handle, uacpi_u16){
    // UNIMPLEMENTED
    return FALSE;
}

void uacpi_kernel_signal_event(uacpi_handle){
    // UNIMPLEMENTED
}

void uacpi_kernel_reset_event(uacpi_handle){
    // UNIMPLEMENTED
}

uacpi_status uacpi_kernel_handle_firmware_request(uacpi_firmware_request*){
    // UNIMPLEMENTED
    return 0;
}

uacpi_status uacpi_kernel_install_interrupt_handler(
    uacpi_u32 irq, uacpi_interrupt_handler, uacpi_handle ctx,
    uacpi_handle *out_irq_handle
){
    // UNIMPLEMENTED
    return 0;
}


uacpi_status uacpi_kernel_uninstall_interrupt_handler(
    uacpi_interrupt_handler, uacpi_handle irq_handle
){
    // UNIMPLEMENTED
    return 0;
}


uacpi_handle uacpi_kernel_create_spinlock(void){
    // UNIMPLEMENTED
    return 0;
}
void uacpi_kernel_free_spinlock(uacpi_handle){
    // UNIMPLEMENTED 
    //return 0;
}

uacpi_cpu_flags uacpi_kernel_lock_spinlock(uacpi_handle){
    // UNIMPLEMENTED
    return 0;
}

void uacpi_kernel_unlock_spinlock(uacpi_handle, uacpi_cpu_flags){
    // UNIMPLEMENTED
}


uacpi_status uacpi_kernel_wait_for_work_completion(void){
    // UNIMPLEMENTED
}

uacpi_status uacpi_kernel_io_read8(
    uacpi_handle, uacpi_size offset, uacpi_u8 *out_value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_io_read16(
    uacpi_handle, uacpi_size offset, uacpi_u16 *out_value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_io_read32(
    uacpi_handle, uacpi_size offset, uacpi_u32 *out_value
){
    // UNIMPLEMENTED
    return 0;
}

uacpi_status uacpi_kernel_io_write8(
    uacpi_handle, uacpi_size offset, uacpi_u8 in_value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_io_write16(
    uacpi_handle, uacpi_size offset, uacpi_u16 in_value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_io_write32(
    uacpi_handle, uacpi_size offset, uacpi_u32 in_value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_pci_read8(
    uacpi_handle device, uacpi_size offset, uacpi_u8 *value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_pci_read16(
    uacpi_handle device, uacpi_size offset, uacpi_u16 *value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_pci_read32(
    uacpi_handle device, uacpi_size offset, uacpi_u32 *value
){
    // UNIMPLEMENTED
    return 0;
}

uacpi_status uacpi_kernel_pci_write8(
    uacpi_handle device, uacpi_size offset, uacpi_u8 value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_pci_write16(
    uacpi_handle device, uacpi_size offset, uacpi_u16 value
){
    // UNIMPLEMENTED
    return 0;
}
uacpi_status uacpi_kernel_pci_write32(
    uacpi_handle device, uacpi_size offset, uacpi_u32 value
){
    // UNIMPLEMENTED
    return 0;
}

uacpi_status uacpi_kernel_pci_device_open(
    uacpi_pci_address address, uacpi_handle *out_handle
){
    // UNIMPLEMENTED
    return 0;
}
void uacpi_kernel_pci_device_close(uacpi_handle){
    // UNIMPLEMENTED
}

uacpi_status uacpi_kernel_io_map(
    uacpi_io_addr base, uacpi_size len, uacpi_handle *out_handle
){
    // UNIMPLEMENTED
    return 0;
}
void uacpi_kernel_io_unmap(uacpi_handle handle){
    // UNIMPLEMENTED
}

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address){
    // UNIMPLEMENTED
    return 0;
}

uacpi_handle uacpi_kernel_create_event(void){
    // UNIMPLEMENTED
    return 0;
}
void uacpi_kernel_free_event(uacpi_handle){
    // UNIMPLEMENTED
}

uacpi_status uacpi_kernel_schedule_work(
    uacpi_work_type, uacpi_work_handler, uacpi_handle ctx
){
    // UNIMPLEMENTED
    return 0;
}