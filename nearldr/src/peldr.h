#ifndef PELDR_H_INCLUDED
#define PELDR_H_INCLUDED








#include "config.h"
#include "externheaders/efi/Uefi.h"


typedef struct _LoaderConfiguration {
    uint64_t mode;
} LoaderConfiguration;


void peldr_load_image(Config conf, LoaderConfiguration config, EFI_HANDLE image);




#endif