
#include <stdlib.h>
#include <assert.h>
int main()
{
	int *a = malloc(8);
	free(a);
	free(a);
	void *b = malloc(8);
	void *c = malloc(8);
	assert((long)b == (long)c);
	return 0;
}
