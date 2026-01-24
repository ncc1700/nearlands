#ifndef ECS_H_INCLUDED
#define ECS_H_INCLUDED



#include "../types.h"
#include "../mm/includes/arena.h"

#define ECS_COMP_SIZE 10

typedef u64 Handle;

typedef enum _ComponentTypes {
    COMP_UNKNOWN,
    COMP_STACK,
    COMP_THREAD
} ComponentTypes;


// typedef struct _Component {
//     void* componentData; // the component is always a single page large, can't be larger
//     ComponentTypes type;
// } __attribute__((packed)) Component;

typedef struct _Entity {
    ComponentTypes componentTypes[ECS_COMP_SIZE];
    Arena arena;
    void* components[ECS_COMP_SIZE];
    u8 componentAmount;
    bool alive;
} Entity;

typedef struct _EntityFreeList {
    u32 index;
    struct _EntityFreeList* next;
} EntityFreeList;

typedef struct _ArcheType {
    ComponentTypes components[ECS_COMP_SIZE];
    Arena arena;
    Entity* entities;
    EntityFreeList* freeList;
    EntityFreeList* initial;
    u64 capacity;
    u64 size;
    u64 sizeInPages;
    u32 index;
    u8 componentAmount;
} ArcheType;

typedef struct _ArcheTypeData {
    ArcheType* archeType;
    u64 index;
} ArcheTypeData;
 


boolean EcsCompareComponents(ComponentTypes* component1, u8 componentAmount1, ComponentTypes* component2, u8 componentAmount2);
u64 EcsGetSizeOfAllComponents(ComponentTypes* components, u8 componentAmount);
u64 EcsEncodeHandle(u32 archTypeIndex, u32 entityIndex);
void EcsDecodeHandle(Handle reference, u32* archTypeIndex, u32* entityIndex);
ArcheTypeData EcsGetArcheType(ComponentTypes* components, u8 componentAmount);
Handle EcsCreateEntity(ComponentTypes* components, u64 componentAmount);
Entity* EcsGetEntity(Handle entityRef);
boolean EcsAddComponentDataToEntity(Handle entityRef, ComponentTypes component, void* componentData);
boolean EcsDeleteEntity(Handle entityHandle);
void* EcsGetComponent(Handle entityRef, ComponentTypes component);


#endif