
#include <stdlib.h>
#include <assert.h>
int main()
{
	void *chunks[7];
	for (int i = 0; i < 7; i++) {
		chunks[i] = malloc(0x30);
	}
	for (int i = 0; i < 7; i++) {
		free(chunks[i]);
	}
	// This has nothing to do with fastbinsY (do not be fooled by the 10) - fake_chunks is just a piece of memory to fulfil allocations (pointed to from fastbinsY)
	long fake_chunks[10] __attribute__((aligned(0x10)));
	fake_chunks[1] = 0x40; // this is the size
	fake_chunks[9] = 0x1234; // nextsize
	void *victim = &fake_chunks[2];
	free(victim);
	void *allocated = calloc(1, 0x30);
	assert(allocated == victim);
}
