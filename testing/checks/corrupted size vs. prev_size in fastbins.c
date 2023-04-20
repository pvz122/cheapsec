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
    //   if (!prev_inuse(p)) {
    //     prevsize = prev_size (p);
    //     size += prevsize;
    //     p = chunk_at_offset(p, -((long) prevsize));
    //     if (__glibc_unlikely (chunksize(p) != prevsize))
    //       malloc_printerr ("corrupted size vs. prev_size in fastbins");
    //     unlink_chunk (av, p);
    //   }

    void *barrier = malloc(0x20);
    void *p1 = malloc(0x410);
    void *p2 = malloc(0x20);

    // fill tcache
    void *fill[7];
    for (int i = 0; i < 7; i++)
        fill[i] = malloc(0x20);
    for (int i = 0; i < 7; i++)
        free(fill[i]);

    // put p2 into fastbin
    free(p2);

    // put p1 into unsorted bin
    free(p1);

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->size = 0xdeadbeef; // c1->size = 0xdeadbeef != c2->prev_size

    // trigger malloc_consolidate
    malloc(0x456); // malloc_consolidate(): corrupted size vs. prev_size in fastbins
    return 0;
}