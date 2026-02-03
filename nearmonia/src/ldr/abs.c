#include "EFI/Base.h"
#include "EFI/UefiSpec.h"
#include "ldr/term.h"
#include <ldr/abs.h>
#include <string.h>







static EFI_SYSTEM_TABLE* globalSysTab = NULL;
static EFI_HANDLE globalHandle = NULL;

#define ACPI_1_TABLE_GUID (EFI_GUID){0xeb9d2d30, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d }}
 
#define ACPI_2_TABLE_GUID (EFI_GUID){0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81 }}
 

void _fltused(){}

// typedef struct {
//     UINT32  Data1;
//     UINT16  Data2;
//     UINT16  Data3;
//     UINT8   Data4[8];
// } GUID;

void LdrSetupAbstractions(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systab){
    globalHandle = handle;
    globalSysTab = systab;
}

boolean LdrCompareGUID(GUID* first, GUID* second){
    if(memcmp(first, second, sizeof(GUID)) == 0){
        return TRUE;
    } else return FALSE;
}   

u64 LdrGetRSDPLocation(){
    GUID acpi1GUID = ACPI_1_TABLE_GUID;
    GUID acpi2GUID = ACPI_2_TABLE_GUID;
    for(u64 i = 0; i < LdrReturnSystemTable()->NumberOfTableEntries; i++){
        EFI_CONFIGURATION_TABLE* table = &LdrReturnSystemTable()->ConfigurationTable[i];
        if(LdrCompareGUID(&table->VendorGuid, &acpi2GUID)){
            LdrTermPrint(TERM_STATUS_PASS, QSTR("Got RSDP: location 0x%x"), table->VendorTable);
            return (u64)table->VendorTable;
        }
    }
    return 0;
}

EFI_SYSTEM_TABLE* LdrReturnSystemTable(){
    return globalSysTab;
}

EFI_HANDLE LdrReturnImagehandle(){
    return globalHandle;
}