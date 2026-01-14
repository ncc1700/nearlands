#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"


typedef enum _RegistryKeyType {
    REG_KEY_I64,
    REG_KEY_U64,
    REG_KEY_STR
} RegistryKeyType;

typedef union _KeyData {
    u64 dU64;
    i64 dI64;
    char* dStr;
} KeyData;

typedef struct _RegistryKey {
    char name[16];
    RegistryKeyType type;
    KeyData data;
} RegistryKey;


typedef struct _RegistryHiveList RegistryHiveList;
typedef struct _RegistryKeyList RegistryKeyList;

typedef struct _RegistryHive {
    char name[16];
    RegistryKeyList* keysBegin;
    RegistryKeyList* keysEnd;
    u32 keyAmount;
    RegistryHiveList* connectionBegin; // addresses of next hive
    RegistryHiveList* connectionEnd; // addresses of next hive
    u32 connectionAmount;
} RegistryHive;

struct _RegistryHiveList {
    RegistryHive hive;
    RegistryHiveList* next;
};

struct _RegistryKeyList {
    RegistryKey key;
    RegistryKeyList* next;
};


RegistryHive* CreateHive(RegistryHive* hive, const char* hiveName){
    RegistryHiveList* node = malloc(sizeof(RegistryHiveList));
    memset(node, 0, sizeof(RegistryHiveList));
    strcpy_s(node->hive.name, 16, hiveName);
    node->next = NULL;
    if(hive->connectionBegin == NULL){
        hive->connectionBegin = node;
    } else {
        hive->connectionEnd->next = node;
    }
    hive->connectionEnd = node;
    hive->connectionAmount++;
    return &node->hive;
}

RegistryHive* OpenHive(RegistryHive* hive, const char* hiveName){
    RegistryHiveList* list = hive->connectionBegin;
    while(list != NULL){
        if(strcmp(list->hive.name, hiveName) == 0){
            return &list->hive;
        }
        list = list->next;
    }
    return NULL;
}

boolean CreateKey(RegistryHive* hive, const char* keyName, RegistryKeyType type, KeyData data){
    RegistryKeyList* node = malloc(sizeof(RegistryKeyList));
    memset(node, 0, sizeof(RegistryKeyList));
    strcpy_s(node->key.name, 16, keyName);
    node->next = NULL;
    node->key.data = data;
    if(hive->keysBegin == NULL){
        hive->keysBegin = node;
    } else {
        hive->keysEnd->next = node;
    }
    hive->keysEnd = node;
    hive->keyAmount++;
    return TRUE;
}

RegistryKey* ReadKey(RegistryHive* hive, const char* keyName){
    RegistryKeyList* list = hive->keysBegin;
    while(list != NULL){
        if(strcmp(list->key.name, keyName) == 0){
            return &list->key;
        }
        list = list->next;
    }
    return NULL;
}



void PrintTreeOfRegistry(i64 index, RegistryHive* hive){
    RegistryHiveList* list = hive->connectionBegin;
    printf("%s\n", hive->name);
    while(list != NULL){
        for(int j = 0; j <= index; j++){
            printf("  -- ");
        }
        PrintTreeOfRegistry(index + 1, &list->hive);
        list = list->next;
    }
}


void FreeRegistry(RegistryHive* hive){
    RegistryHiveList* hiveList = hive->connectionBegin;
    while(hiveList != NULL){
        FreeRegistry(&hiveList->hive);
        hiveList = hiveList->next;
    }

    RegistryKeyList* keyList = hive->keysBegin;
    while(keyList != NULL){
        if(keyList->key.type == REG_KEY_STR){
            free(keyList->key.data.dStr);
        }
        RegistryKeyList* prev = keyList;
        keyList = keyList->next;
        free(prev);
    }

    hiveList = hive->connectionBegin;
    while(hiveList != NULL){
        RegistryHiveList* prev = hiveList;
        hiveList = hiveList->next;
        free(prev);
    }
}


i32 main(int argc, char* argv[]){
    if(argc < 2){
        printf("reggen [hive name]\n");
        return -1;
    }
    RegistryHive root = {0};
    strcpy_s(root.name, 16, argv[1]);
    RegistryHive* stuff = CreateHive(&root, "stuff");
    RegistryHive* poo = CreateHive(stuff, "poo");
    RegistryHive* poo2 = CreateHive(stuff, "poo2");
    RegistryHive* stuff2 = CreateHive(&root, "stuff2");
    CreateKey(stuff, "h", REG_KEY_I64, (KeyData){.dI64 =  -1});
    printf("key: %lld\n", ReadKey(stuff, "h")->data.dI64);
    PrintTreeOfRegistry(0, &root);
    FreeRegistry(&root);
}