#include <stdlib.h>
#include <assert.h>

int main()
{
	void *ptrs[8];
	for (int i = 0; i < 8; i++) {
		ptrs[i] = malloc(8);
	}
	for (int i = 0; i < 7; i++) {
		free(ptrs[i]);
	}

	int *a = calloc(1, 8);
	int *b = calloc(1, 8);
	int *c = calloc(1, 8);


	free(a);

	// free(a);

	free(b);

	free(a);

	a = calloc(1, 8);
	b = calloc(1, 8);
	c = calloc(1, 8);

	assert(a == c);

	return 0;
}
