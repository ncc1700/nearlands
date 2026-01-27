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