#include <stdlib.h>
#include <assert.h>

int main()
{
	// reference: https://valsamaras.medium.com/the-toddlers-introduction-to-heap-exploitation-fastbin-dup-consolidate-part-4-2-ce6d68136aa8

	void *p1 = calloc(1, 0x40);

	free(p1);

	void *p3 = malloc(0x400);

	assert(p1 == p3);

	free(p1);

	void *p4 = malloc(0x400);

	assert(p4 == p3);

	return 0;
}
