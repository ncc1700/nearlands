#ifndef ELFLDR_H_INCLUDED
#define ELFLDR_H_INCLUDED











#include <stdint.h>
#include "config.h"
#include "externheaders/efi/Uefi.h"



void elfldr_load_image(Config conf, int mode, EFI_HANDLE image);









#endif