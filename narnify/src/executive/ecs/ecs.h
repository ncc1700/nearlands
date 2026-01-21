#ifndef ECS_H_INCLUDED
#define ECS_H_INCLUDED



#include "../../types.h"
#include "../../arch/includes/arena.h"

#define ECS_COMP_SIZE 20

typedef u64 Handle;

typedef enum _ComponentTypes {
    COMP_UNKNOWN,
    COMP_STACK,
    COMP_THREAD
} ComponentTypes;

typedef struct _Component {
    void* componentData;
    ComponentTypes type;
} __attribute__((packed)) Component;

typedef struct _Entity {
    Component components[ECS_COMP_SIZE];
    Handle entityRef;
    u8 componentAmount;
} Entity;

typedef struct _ArcheType {
    Component components[ECS_COMP_SIZE];
    u8 componentAmount;
    Entity* entities;
    u64 index;
    u64 capacity;
    u64 size;
    u64 sizeInPages;
} ArcheType;

typedef struct _ArcheTypeData {
    ArcheType* archeType;
    u64 index;
} ArcheTypeData;
 


boolean EcsCompareComponents(Component* component1, Component* component2, u8 componentAmount);
u64 EcsGetSizeOfAllComponents(Component* components, u8 componentAmount);
u64 EcsEncodeHandle(u32 archTypeIndex, u32 entityIndex);
void EcsDecodeHandle(Handle reference, u32* archTypeIndex, u32* entityIndex);
ArcheTypeData EcsGetArcheType(Component* components, u8 componentAmount);
Handle EcsCreateEntity(Component* components, u64 componentAmount);
Entity* EcsGetEntity(Handle entityRef);
Component* EcsGetComponent(Handle entityRef, ComponentTypes component);


#endif