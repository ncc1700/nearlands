#include <qol/qstring.h>
#include <nrstatus.h>






static const char* statusToString[] = {
    [STATUS_SUCCESS] = "The operation completed successfully",
    [STATUS_UNKNOWN] = "Unknown NearStatus code",
    [STATUS_OUT_OF_MEMORY] = "The system has ran out of memory",
    [STATUS_INIT_FAIL] = "Failed initializing component",
    [STATUS_CANT_CREATE_ECS_COMP] = "Couldn't create ECS component",
    [STATUS_CANT_CREATE_ENTITY] = "Couldn't create entity",
    [STATUS_NOT_ENOUGH_MEMORY] = "Theres not enough memory avaliable",
    [STATUS_NO_VALID_MEMORY] = "Not enough valid memory present",
    [STATUS_NOT_FOUND] = "Couldn't find it",
    [STATUS_INVALID_PML_ENTRY] = "Invalid PML entry",
    [STATUS_NOT_MAPPED] = "Memory not mapped",
    [STATUS_UACPI_INIT_FAIL] = "Couldn't initialize uACPI",
    [STATUS_ACPI_NAMESPACE_FAIL] = "Couldn't load ACPI namespace",
    [STATUS_ACPI_NAMESPACE_INIT_FAIL] = "Couldn't initialize ACPI namespace",
    [STATUS_REBOOT_FAILED] = "Couldn't reboot the machine",
    [STATUS_SHUTDOWN_FAILED] = "Couldn't shut down the machine",
    [STATUS_INVALID_ADDRESS] = "Tried to access invalid address",
    [STATUS_ACCESS_DENIED] = "Access is denied",
    [STATUS_CANT_FIND_HEADER] = "Couldn't find header",
    [STATUS_PMM_FAIL] = "Failed setting up the physical memory manager",
    [STATUS_VMM_FAIL] = "Failed setting up the virtual memory manager",
    [STATUS_GALLOC_FAIL] = "Failed setting up the general memory allocator",
    [STATUS_TOO_LARGE] = "The item is too large",
};



const char* KeNrStatusToCString(NearStatus status){
    u64 arrSize = ARR_LENGTH(statusToString);
    if((u64)status >= arrSize){
        return "";
    }
    return statusToString[status];
}

QString KeNrStatusToString(NearStatus status){
    return QSTR(KeNrStatusToCString(status));
}