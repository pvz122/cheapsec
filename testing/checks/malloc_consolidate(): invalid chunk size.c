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
    // 	  unsigned int idx = fastbin_index (chunksize (p));
    //   if ((&fastbin (av, idx)) != fb)
    //     malloc_printerr ("malloc_consolidate(): invalid chunk size");

    // target fastbin chunk
    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);

    // fill tcache
    void *fill[7];
    for (int i = 0; i < 7; i++)
        fill[i] = malloc(0x20);
    for (int i = 0; i < 7; i++)
        free(fill[i]);

    // put p1, p2 into fastbin
    free(p1);
    free(p2);
    // fastbin -> p2 -> p1

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->size = 0xdeadbeef; // invalid chunk size

    // trigger malloc_consolidate
    malloc(0x456); // malloc_consolidate(): invalid chunk size
    return 0;
}