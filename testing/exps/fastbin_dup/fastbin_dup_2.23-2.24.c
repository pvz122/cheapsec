#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
	int *a = malloc(8);
	int *b = malloc(8);
	int *c = malloc(8);


	free(a);

	// free(a);

	free(b);

	free(a);

	a = malloc(8);
	b = malloc(8);
	c = malloc(8);

	assert(a == c);

	return 0;
}
