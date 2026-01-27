#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED






#include "../types.h"
#include "../ecs/ecs.h"
typedef enum _ThreadState {
    THREAD_STATE_STARTING,
    THREAD_STATE_SUSPEND,
    THREAD_STATE_RUNNING,
    THREAD_STATE_WAITING,
    THREAD_STATE_ENDING
} ThreadState;

typedef struct _UserThread {
    u64 (*SysThreadEntry)(void* data);
    u64 userStackTop;
    u64 userStackBottom;
    u64 kernelStackTop;
    u64 kernelStackBottom;
    ThreadState tState;
    struct _Thread* next;
} UserThread;

typedef struct _KernThread {
    u64 (*SysThreadEntry)(void* data);
    u64 kernelStackTop;
    u64 kernelStackBottom;
    ThreadState tState;
    struct _Thread* next;
} KernThread;

typedef struct _UserThreadComponent {
    UserThread* initialThread;
    Handle entity;
} UserThreadComponent;

typedef struct _KernThreadComponent {
    KernThread* initialThread;
    Handle entity;
} KernThreadComponent;


boolean KeInitializeThreadComponent();
u16 KeReturnKtCompIndex();
u16 KeReturnUtCompIndex();


#endif