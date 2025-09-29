#include "core/tools/debugtools.h"
#include "hal/includes/halinit.h"
#include "limine.h"
#include <stddef.h>


__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);


__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    LIMINE_FRAMEBUFFER_REQUEST,
    0
};


__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    uint8_t *restrict pdest = (uint8_t *restrict)dest;
    const uint8_t *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}


void kernel_entry(void) {
    DKPRINTLN("Kernel Entrypoint Reached");
    setup_hal();
    DKPRINTLN("Kernel HAL loaded successfully");
    if(LIMINE_BASE_REVISION_SUPPORTED == 0){
        DKPRINTLN("UNKNOWN BASE REVISION");
        while(1){continue;}
    }
    if(framebuffer_request.response == NULL){
        while(1){continue;}
    }
    if(framebuffer_request.response->framebuffer_count < 1){
        while(1){continue;}
    }
    
    struct limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];
    for(int x = 0; x < 10; x++){
        for(int y = 0; y < 30; y++){
            volatile uint32_t *fbp = fb->address;
            fbp[y * (fb->pitch / 4) + x] = 0xffffaf;
        }
    }
    
    while(1){continue;}
}