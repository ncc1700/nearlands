#include "ecs.h"
#include "../../qol.h"
#include "../../arch/includes/pmm.h"

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


boolean EcsCompareComponents(Component* component1, Component* component2, u8 componentAmount){
    for(u64 i = 0; i < componentAmount; i++){
        if(component1[i].type != component2[i].type){
            return FALSE;
        }
    }
    return TRUE;
}

u64 EcsGetSizeOfAllComponents(Component* components, u8 componentAmount){
    u64 componentSize = 0;
    for(u64 i = 0; i < componentAmount; i++){
        componentSize += componentSizeArr[components[i].type];
    }
    return componentSize;
}

Handle EcsEncodeHandle(u32 archTypeIndex, u32 entityIndex){
    return ((u64)archTypeIndex << 32) | entityIndex;
}

void EcsDecodeHandle(Handle handle, u32* archTypeIndex, u32* entityIndex){
    *archTypeIndex = (handle >> 32);
    *entityIndex = (u32)(handle);
}



ArcheTypeData EcsGetArcheType(Component* components, u8 componentAmount){
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
        if(EcsCompareComponents(components, archeTypeArray[i].components,
                                    componentAmount) == TRUE)
        {
            TermPrint(TERM_STATUS_INFO, "found previous archtype, returning..");
            return (ArcheTypeData){&archeTypeArray[i], i};
        }
    }
    memset((void*)&archeTypeArray[ataIndex], 0, sizeof(ArcheType));  
    memcpy((void*)&archeTypeArray[ataIndex].components, components, 
                        sizeof(Component) * componentAmount);  
    archeTypeArray[ataIndex].componentAmount = componentAmount;
    u64 prevIndex = ataIndex;
    ataIndex++;
    return (ArcheTypeData){&archeTypeArray[prevIndex], prevIndex};
}

Handle EcsCreateEntity(Component* components, u64 componentAmount){
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
    u64 enIndex = archeType->index;
    archeType->index++;
    archeType->entities[enIndex].componentAmount = componentAmount;
    memcpy(archeType->entities[enIndex].components, components, 
                componentAmount * sizeof(Component));
    
    return EcsEncodeHandle(archeTypeData.index, enIndex);
}

Entity* EcsGetEntity(Handle entityRef){
    u32 archTypeIndex;
    u32 entityIndex;
    EcsDecodeHandle(entityRef, &archTypeIndex, &entityIndex);
    if(archTypeIndex >= ataIndex){
        return NULL;
    }
    ArcheType* archType = &archeTypeArray[archTypeIndex];
    if(entityIndex >= archType->index){
        return NULL;
    }
    return &archType->entities[entityIndex];
}


Component* EcsGetComponent(Handle entityRef, ComponentTypes component){
    Entity* entity = EcsGetEntity(entityRef);
    if(entity == NULL) return NULL;
    for(u8 i = 0; i < entity->componentAmount; i++){
        if(entity->components[i].type == component){
            return &entity->components[i];
        }
    }
    return NULL;
}