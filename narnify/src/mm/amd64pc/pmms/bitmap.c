

#include "bootinfo.h"
#include "ke/spinlock.h"
#include "ke/term.h"
#include "nrstatus.h"
#include "types.h"
#include <mm/pmm.h>
#include <qol/qmem.h>





#ifdef BITMAP_PMM



static u64 pageCount = 0;
static u64 usablePageCount = 0;
static u64 topAddr = 0;

static u64 bitmapSize = 0;
static u8* bitmap = NULL;
static BootMemoryMap* bMemMap = NULL;

static u64 pageUsed = 0;
static SpinLock bitmapLock = {0};

#define SET_BITMAP(bit) (bitmap[bit / 8] |= (1u << (bit % 8)))
#define CLEAR_BITMAP(bit) (bitmap[bit / 8] &= ~(1u << (bit % 8)))
#define GET_BIT_IN_BITMAP(bit) (bitmap[bit / 8] & (1 << (bit % 8)))


#define SET TRUE
#define CLEAR FALSE

#define INCREMENT_PAGE_USAGE() if(pageUsed < pageCount) pageUsed++;
#define DECREMENT_PAGE_USAGE() if(pageUsed > 0) pageUsed--;


static u64 GetPageLocationFromAddress(u64 address){
    u64 trueAddressOffset = address;
    return trueAddressOffset / PAGE_SIZE;
}

static u64 GetAddressFromPageLocation(u64 pageLocation){
    u64 addrBeforeAddition = pageLocation * PAGE_SIZE;
    return addrBeforeAddition;
}



static inline void MarkBitInBitmapFromAddress(u64 address, boolean setOrClear){
    u64 page = GetPageLocationFromAddress(address);
    if(setOrClear == SET){
        SET_BITMAP(page);
    } else {
        CLEAR_BITMAP(page);
    }
}

static boolean GetBit(u64 address){
    return GET_BIT_IN_BITMAP(GetPageLocationFromAddress(address));
}

NearStatus MmInitPhysicalMemoryManager(BootMemoryMap* memMap){
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        boolean isUseable = FALSE;
        if(memMap->memEntries[i].type == BOOT_MEM_TYPE_FREE) isUseable = TRUE;
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        u64 sizeInPages = IN_PAGES(size);
        for(u64 j = 0; j < sizeInPages; j++){
            pageCount++;
            if(isUseable == TRUE) usablePageCount++;
        }
    }

    bitmapSize = pageCount / 8;
    if(bitmapSize == 0){ // we could calculate it diffrently but i don't wanna
        return STATUS_NO_VALID_MEMORY;
    }

    u64 bitmapSizeInPages = IN_PAGES(bitmapSize);
    
    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        u64 sizeInPages = IN_PAGES(size);
        if(size >= bitmapSize){
            bitmap = (u8*)base;
            break;
        }
    }
    memset(bitmap, 0xFF, (bitmapSize));

    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type != BOOT_MEM_TYPE_FREE) continue;
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        u64 sizeInPages = IN_PAGES(size);
        for(u64 j = 0; j < sizeInPages; j++){
            MarkBitInBitmapFromAddress((base + (j * PAGE_SIZE)), 
                                            CLEAR);
        }
    }

    for(u64 i = 0; i < memMap->amountOfEntries; i++){
        if(memMap->memEntries[i].type == BOOT_MEM_TYPE_FREE) continue;
        u64 base = memMap->memEntries[i].base;
        u64 size = memMap->memEntries[i].size;
        u64 sizeInPages = IN_PAGES(size);
        for(u64 j = 0; j < sizeInPages; j++){
            MarkBitInBitmapFromAddress((base + (j * PAGE_SIZE)), 
                                            SET);
        }
    }
    
    for(u64 i = 0; i < bitmapSizeInPages; i++){
        MarkBitInBitmapFromAddress(((u64)bitmap + (i * PAGE_SIZE)), SET);
    }

    return STATUS_SUCCESS;
}

void* MmAllocateSinglePage(){

    for(u64 i = 0; i < pageCount; i++){
        if(GET_BIT_IN_BITMAP(i) == CLEAR){
            void* addr = (void*)(GetAddressFromPageLocation(i));
            if(addr == 0x0){
                continue;
            }
            SET_BITMAP(i);
            INCREMENT_PAGE_USAGE();
            return addr;
        }
    }
    return NULL; // we ran out of memory =(
}

void* MmAllocateMultiplePages(u64 amount){
    u64 trueAmount = pageCount - amount;
    for(u64 i = 0; i <= trueAmount; i++){
        void* addr = (void*)(GetAddressFromPageLocation(i));
        if(GET_BIT_IN_BITMAP(i) == CLEAR){
            boolean valid = TRUE;
            if(addr == 0x0){
                continue;
            }
            for(u64 j = 0; j < amount; j++){
                u64 k = i + j;
                if(GET_BIT_IN_BITMAP(k) == SET){
                    valid = FALSE;
                    break;
                }
            }
            if(valid == TRUE){
                SET_BITMAP(i);
                for(u64 j = 0; j < amount; j++){
                    u64 k = i + j;
                    SET_BITMAP(k);
                    INCREMENT_PAGE_USAGE();
                }
                return addr;
            }
        }
    }
    return NULL;
}

NearStatus MmFreeSinglePage(void* pageAddress){
    u64 index = GetPageLocationFromAddress((u64)pageAddress);
    CLEAR_BITMAP(index);
    DECREMENT_PAGE_USAGE();
    return STATUS_SUCCESS;
}

NearStatus MmFreeMultiplePages(void* pageAddress, u64 amount){
    u64 index = GetPageLocationFromAddress((u64)pageAddress);
    for(u64 i = 0; i < amount; i++){
        u64 k = index + i;
        CLEAR_BITMAP(k);
        DECREMENT_PAGE_USAGE();
    }
    return STATUS_SUCCESS;
}

u64 MmReturnPageUsed(){
    return pageUsed;
}

u64 MmReturnPageAmount(){
    return usablePageCount;
}


#endif