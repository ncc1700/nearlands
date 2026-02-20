#include "ke/term.h"
#include "mm/pmm.h"
#include "nrstatus.h"
#include "uacpi/event.h"
#include "uacpi/sleep.h"
#include "uacpi/status.h"
#include "uacpi/tables.h"
#include <ke/panic.h>
#include <uacpi/uacpi.h>
#include <acpi/acpi.h>
#include <uacpi/types.h>



NearStatus AcpiInitSystem(){
    u64 size = MmReturnPageUsed();
    KeTermPrint(TERM_STATUS_INFO, QSTR("page used is %d\n"), size);
    uacpi_status status = uacpi_initialize(0);
    if(uacpi_unlikely_error(status)){
        return STATUS_UACPI_INIT_FAIL;
    }
    status = uacpi_namespace_load();
    if(uacpi_unlikely_error(status)){
        return STATUS_ACPI_NAMESPACE_FAIL;
    }
    status = uacpi_namespace_initialize();
    if(uacpi_unlikely_error(status)){
        return STATUS_ACPI_NAMESPACE_FAIL;
    }
    status = uacpi_finalize_gpe_initialization();
    if(uacpi_unlikely_error(status)){
        return STATUS_ACPI_GPE_FAIL;
    }
    size = MmReturnPageUsed();
    KeTermPrint(TERM_STATUS_INFO, QSTR("page finally used is %d\n"), size);
    return STATUS_SUCCESS;
}



void AcpiShutdownSystem(){
    uacpi_prepare_for_sleep_state(UACPI_SLEEP_STATE_S5);
    uacpi_enter_sleep_state(UACPI_SLEEP_STATE_S5);
    // shouldn't go here
    KePanic(STATUS_SHUTDOWN_FAILED);
}

void AcpiRebootSystem(){
    uacpi_prepare_for_sleep_state(UACPI_SLEEP_STATE_S5);
    uacpi_reboot();
    // shouldn't go here
    KePanic(STATUS_REBOOT_FAILED);
}

