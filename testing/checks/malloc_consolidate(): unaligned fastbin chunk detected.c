#include <stdlib.h>
#include <assert.h>

typedef struct malloc_chunk {

    size_t      prev_size;  /* Size of previous chunk (if free).  */
    size_t      size;       /* Size in bytes, including overhead. */

    struct malloc_chunk *fd;         /* double links -- used only if free. */
    struct malloc_chunk *bk;

    /* Only used for large blocks: pointer to next larger size.  */
    struct malloc_chunk *fd_nextsize; /* double links -- used only if free. */
    struct malloc_chunk *bk_nextsize;
} chunk_t;
typedef chunk_t *chunk_p;

int main(void)
{
    //       maxfb = &fastbin (av, NFASTBINS - 1);
    //   fb = &fastbin (av, 0);
    //   do {
    //     p = atomic_exchange_acquire (fb, NULL);
    //     if (p != 0) {
    //       do {
    // 	{
    // 	  if (__glibc_unlikely (misaligned_chunk (p)))
    // 	    malloc_printerr ("malloc_consolidate(): "
    // 			     "unaligned fastbin chunk detected");

    // target fastbin chunk
    void *p = malloc(0x20);

    // fill tcache
    void *fill[7];
    for (int i = 0; i < 7; i++)
        fill[i] = malloc(0x20);
    for (int i = 0; i < 7; i++)
        free(fill[i]);

    // put p into fastbin
    free(p);

    char buffer[0x100] = { 0 };
    chunk_p fake = (chunk_p)(&buffer[1]); // unaligned fake fastbin chunk
    fake->size = 0x30 | 0x1;

    chunk_p c = (chunk_p)((char *)p - 0x10);
    c->fd = fake;

    // trigger malloc_consolidate
    malloc(0x456); // malloc_consolidate(): unaligned fastbin chunk detected
    return 0;
}