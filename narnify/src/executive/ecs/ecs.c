#include "ecs.h"
#include "../../qol.h"
#include "../../arch/includes/pmm.h"
#include "../../arch/includes/heap.h"

#include "components/stackcmp.h"
#include "components/threadcmp.h"
#include "../../term.h"




static const size_t componentSizeArr[] = {
    [COMP_THREAD] = sizeof(ThreadComponent),
    [COMP_STACK] = sizeof(StackComponent)
};

static ArcheType* archeTypeArray = NULL;
static u64 ataIndex = 0;
static u64 ataCapacity = 0;
static u64 ataSize = 0;
static u64 ataSizeInPages = 0;


boolean EcsCompareComponents(ComponentTypes* component1, u8 componentAmount1, ComponentTypes* component2, u8 componentAmount2){
    if(componentAmount1 != componentAmount2) return FALSE;
    u64 componentAmount = componentAmount1;
    for(u64 i = 0; i < componentAmount; i++){
        if(component1[i] != component2[i]){
            return FALSE;
        }
    }
    return TRUE;
}

u64 EcsGetSizeOfAllComponents(ComponentTypes* components, u8 componentAmount){
    u64 componentSize = 0;
    for(u64 i = 0; i < componentAmount; i++){
        componentSize += componentSizeArr[components[i]];
    }
    return componentSize;
}

Handle EcsEncodeHandle(u32 archeTypeIndex, u32 entityIndex){
    return ((u64)archeTypeIndex << 32) | entityIndex;
}

void EcsDecodeHandle(Handle handle, u32* archeTypeIndex, u32* entityIndex){
    *archeTypeIndex = (handle >> 32);
    *entityIndex = (u32)(handle);
}



ArcheTypeData EcsGetArcheType(ComponentTypes* components, u8 componentAmount){
    if(ataIndex >= ataCapacity){
        TermPrint(TERM_STATUS_INFO, "Expanding ArcheTypes Array");
        u64 newAtCapacity = 0;
        u64 newAtSize = 0;
        u64 newAtSizeInPages = 0;
        if(ataCapacity == 0) newAtCapacity = 15; 
        else newAtCapacity = ataCapacity * 3;
        newAtSize = newAtCapacity * sizeof(ArcheType);
        newAtSizeInPages = (newAtSize + (PAGE_SIZE - 1)) / PAGE_SIZE;
        archeTypeArray = MmReallocatePages(archeTypeArray, 
                        ataSizeInPages, newAtSizeInPages);
        if(archeTypeArray == NULL){
            // panic bc no more memory
            QolPanic("OUT OF MEMORY: from EcsGetArcheType()");
        }
        ataSize = newAtSize;
        ataCapacity = newAtCapacity;
        ataSizeInPages = newAtSizeInPages;
    }
    for(u64 i = 0; i < ataIndex; i++){
        if(archeTypeArray[i].componentAmount != componentAmount){
            continue;
        }
        if(EcsCompareComponents(components, componentAmount, 
                                    archeTypeArray[i].components,
                                    archeTypeArray[i].componentAmount) == TRUE)
        {
            //TermPrint(TERM_STATUS_INFO, "found previous archeType, returning..");
            return (ArcheTypeData){&archeTypeArray[i], i};
        }
    }
    memset((void*)&archeTypeArray[ataIndex], 0, sizeof(ArcheType));  
    for(u8 i = 0; i < componentAmount; i++){
        archeTypeArray[ataIndex].components[i] = components[i];
    }
    
    archeTypeArray[ataIndex].componentAmount = componentAmount;
    archeTypeArray[ataIndex].initial = NULL;
    archeTypeArray[ataIndex].freeList = NULL;
    u64 prevIndex = ataIndex;
    ataIndex++;
    return (ArcheTypeData){&archeTypeArray[prevIndex], prevIndex};
}

Handle EcsCreateEntity(ComponentTypes* components, u64 componentAmount){
    ArcheTypeData archeTypeData = EcsGetArcheType(components, componentAmount);
    ArcheType* archeType = archeTypeData.archeType;
    if(archeType->index >= archeType->capacity){
        TermPrint(TERM_STATUS_INFO, "Expanding Entity Array in Archetype");
        TermPrint(TERM_STATUS_INFO, "ataCapacity is %d, ataIndex is %d", 
            ataCapacity, ataIndex);
        u64 newCapacity = 0;
        u64 newSize = 0;
        u64 newSizeInPages = 0;
        if(archeType->capacity == 0) newCapacity = 15; 
        else newCapacity = archeType->capacity * 3;
        newSize = newCapacity * sizeof(Entity);
        newSizeInPages = (newSize + (PAGE_SIZE - 1)) / PAGE_SIZE;
        archeType->entities = MmReallocatePages(archeType->entities, 
                                archeType->sizeInPages, newSizeInPages);
        if(archeType->entities == NULL){
            // panic bc no more memory
            QolPanic("OUT OF MEMORY: from EcsCreateEntity()");
        }
        archeType->size = newSize;
        archeType->capacity = newCapacity;
        archeType->sizeInPages = newSizeInPages;
    }
    u64 enIndex = 0;
    if(archeType->initial != NULL){
        TermPrint(TERM_STATUS_INFO, "reusing deleted entity..");
        EntityFreeList* node = archeType->initial;
        archeType->initial = archeType->initial->next;
        enIndex = node->index;
        MmFreeFromHeap(node, sizeof(EntityFreeList));
    } else {
        enIndex = archeType->index;
        archeType->index++;
    }
    u64 size = EcsGetSizeOfAllComponents(components, componentAmount);
    memset(&archeType->entities[enIndex], 0, sizeof(Entity));
    MmCreateArena(&archeType->entities[enIndex].arena, size);
    archeType->entities[enIndex].componentAmount = componentAmount;
    for(u64 i = 0; i < componentAmount; i++){
        archeType->entities[enIndex].componentTypes[i] = components[i];
        archeType->entities[enIndex].components[i] = MmPushMemoryFromArena(&archeType->entities[enIndex].arena,
                                                            componentSizeArr[components[i]]);
        if(archeType->entities[enIndex].components[i] == NULL){
            TermPrint(TERM_STATUS_INFO, "failed at index %d", i);
            QolPanic("OUT OF MEMORY: from EcsCreateEntity()");
        }
    }
    archeType->entities[enIndex].alive = TRUE;
    return EcsEncodeHandle(archeTypeData.index, enIndex);
}

Entity* EcsGetEntity(Handle entityRef){
    u32 archeTypeIndex;
    u32 entityIndex;
    EcsDecodeHandle(entityRef, &archeTypeIndex, &entityIndex);
    if(archeTypeIndex >= ataIndex){
        return NULL;
    }
    ArcheType* archeType = &archeTypeArray[archeTypeIndex];
    if(entityIndex >= archeType->index){
        return NULL;
    }
    if(archeType->entities[entityIndex].alive == FALSE){
        return NULL;
    }
    return &archeType->entities[entityIndex];
}

boolean EcsAddComponentDataToEntity(Handle entityRef, ComponentTypes component, void* componentData){
    Entity* entity = EcsGetEntity(entityRef);
    if(entity == NULL) return FALSE;
    i64 index = -1;
    for(u8 i = 0; i < entity->componentAmount; i++){
        if(entity->componentTypes[i] == component){
            index = i;
            break;
        }
    }
    if(index == -1){
        return FALSE;
    }
    if(entity->components[index] == NULL) return FALSE;
    memcpy(entity->components[index], componentData, componentSizeArr[component]);
    return TRUE;
}

boolean EcsDeleteEntity(Handle entityHandle){
    Entity* entity = EcsGetEntity(entityHandle);
    if(entity == NULL) return FALSE;
    MmDestroyArena(&entity->arena);
    for(u64 i = 0; i < entity->componentAmount; i++){
        entity->componentTypes[i] = 0;
    }
    entity->alive = FALSE;
    u32 archeTypeIndex;
    u32 entityIndex;
    EcsDecodeHandle(entityHandle, &archeTypeIndex, &entityIndex);
    ArcheType* archeType = &archeTypeArray[archeTypeIndex];

    EntityFreeList* node = MmAllocateFromHeap(sizeof(EntityFreeList));
    if(node == NULL){
        QolPanic("Failure to create node for ECS");
    }
    node->index = entityIndex;
    node->next = NULL;

    if(archeType->initial == NULL){
        archeType->initial = node;
        archeType->freeList = node;
    } else {
        archeType->freeList->next = node;
        archeType->freeList = archeType->freeList->next;
    }
    return TRUE;
}


void* EcsGetComponent(Handle entityRef, ComponentTypes component){
    Entity* entity = EcsGetEntity(entityRef);
    if(entity == NULL) return NULL;
    for(u8 i = 0; i < entity->componentAmount; i++){
        if(entity->componentTypes[i] == component){
            return entity->components[i];
        }
    }
    return NULL;
}