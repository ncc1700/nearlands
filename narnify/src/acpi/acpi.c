#include "uacpi/sleep.h"
#include "uacpi/status.h"
#include <ke/panic.h>
#include <uacpi/uacpi.h>
#include <acpi/acpi.h>
#include <uacpi/types.h>

/**
    TODO: implement

*/


boolean AcpiInitSystem(){
    uacpi_status status = uacpi_initialize(0);
    if(uacpi_unlikely_error(status)){
        KePanic(QSTR("couldn't initialize uACPI!"));
    }
    status = uacpi_namespace_load();
    if(uacpi_unlikely_error(status)){
        KePanic(QSTR("couldn't load uACPI namespace!"));
    }
    status = uacpi_namespace_initialize();
    if(uacpi_unlikely_error(status)){
        KePanic(QSTR("couldn't initialize uACPI namespace!"));
    }
    return TRUE;
}

void AcpiShutdownSystem(){
    uacpi_prepare_for_sleep_state(UACPI_SLEEP_STATE_S5);
    uacpi_enter_sleep_state(UACPI_SLEEP_STATE_S5);
    KePanic(QSTR("failure to initiate ACPI shutdown"));
}

void AcpiRebootSystem(){
    uacpi_prepare_for_sleep_state(UACPI_SLEEP_STATE_S5);
    uacpi_reboot();
    // shouldn't go here
    KePanic(QSTR("failure to initiate ACPI reboot"));
}