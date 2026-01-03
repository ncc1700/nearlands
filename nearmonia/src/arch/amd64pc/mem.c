#include "../includes/mem.h"
#include "../../qol.h"
#include "../../term.h"





MemoryMap* LdrMmRetrieveCurrentMemoryMap(boolean exitBootServices){
    u64 memMapSize = 0;
    u64 mapKey = 0;
    u64 descSize = 0;
    u32 descVer = 0;
    EFI_MEMORY_DESCRIPTOR* efiMemMap = NULL;
    MemoryMap* memMap = NULL;
    // we get the size by passing well nothing
    QolReturnSystemTable()->BootServices->GetMemoryMap(&memMapSize, 
                                        efiMemMap, &mapKey, &descSize, 
                                        &descVer);
    memMapSize += descSize * 8;

    EFI_STATUS status = QolReturnSystemTable()->BootServices->AllocatePool(EfiLoaderData, 
        memMapSize, (void**)&efiMemMap);
    if(status != EFI_SUCCESS){
        TermPrint(TERM_STATUS_ERROR, "Couldn't allocate pool for Efi Memory Map, 0x%x", status);
        return NULL;
    }

    status = QolReturnSystemTable()->BootServices->AllocatePool(EfiLoaderData, 
        sizeof(MemoryMap), (void**)&memMap);
    if(status != EFI_SUCCESS){
        TermPrint(TERM_STATUS_ERROR, "Couldn't allocate pool for Nearmonia Memory Map, 0x%x", status);
        QolReturnSystemTable()->BootServices->FreePool(efiMemMap);
        return NULL;
    }

    QolReturnSystemTable()->BootServices->SetMem(memMap, 
                        sizeof(MemoryMap), 0);

    status = QolReturnSystemTable()->BootServices->GetMemoryMap(&memMapSize, efiMemMap, &mapKey, 
                            &descSize, &descVer);
    if(status != EFI_SUCCESS){
        TermPrint(TERM_STATUS_ERROR, "Couldn't get EFI memory map, 0x%x", status);
        QolReturnSystemTable()->BootServices->FreePool(efiMemMap);
        QolReturnSystemTable()->BootServices->FreePool(memMap);
        return NULL;
    }
    u64 amountOfEntries = memMapSize / descSize;
    status = QolReturnSystemTable()->BootServices->AllocatePool(EfiLoaderData, 
                        amountOfEntries * sizeof(MemoryMapEntry), (void**)&memMap->memEntries);
    if(status != EFI_SUCCESS){
        TermPrint(TERM_STATUS_ERROR, "Couldn't get EFI memory map, 0x%x", status);
        QolReturnSystemTable()->BootServices->FreePool(efiMemMap);
        QolReturnSystemTable()->BootServices->FreePool(memMap);
        return NULL;
    }
    QolReturnSystemTable()->BootServices->SetMem(memMap->memEntries, 
                        amountOfEntries * sizeof(MemoryMapEntry), 0);

    memMap->amountOfEntries = amountOfEntries;
    for(u64 i = 0; i < amountOfEntries; i++){
        EFI_MEMORY_DESCRIPTOR* efiMemDesc = (EFI_MEMORY_DESCRIPTOR*)((u8*)efiMemMap + (i * descSize));
        memMap->memEntries[i].base = efiMemDesc->PhysicalStart;
        memMap->memEntries[i].size = efiMemDesc->NumberOfPages * 4096;
        memMap->sizeOfEntireMemory += memMap->memEntries[i].size;
        switch (efiMemDesc->Type) {
            case EfiConventionalMemory:
                memMap->memEntries[i].type = MEM_TYPE_FREE;
                break;
            case EfiACPIMemoryNVS:
            case EfiACPIReclaimMemory:
                memMap->memEntries[i].type = MEM_TYPE_ACPI;
                break;
            default:
                memMap->memEntries[i].type = MEM_TYPE_USED;
                break;
        }
    }
    if(exitBootServices == TRUE){
        QolReturnSystemTable()->BootServices->ExitBootServices(QolReturnImagehandle(), mapKey);
    }
    return memMap;
}   
