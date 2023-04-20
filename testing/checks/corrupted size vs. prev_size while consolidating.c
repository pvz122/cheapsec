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
    //       /* consolidate backward */
    //   if (!prev_inuse(p)) {
    //     prevsize = prev_size (p);
    //     size += prevsize;
    //     p = chunk_at_offset(p, -((long) prevsize));
    //     if (__glibc_unlikely (chunksize(p) != prevsize))
    //       malloc_printerr ("corrupted size vs. prev_size while consolidating");
    //     unlink_chunk (av, p);
    //   }

    // target unsorted bin chunks
    void *p1 = malloc(0x410);
    void *p2 = malloc(0x410);
    void *p3 = malloc(0x410);

    free(p1);

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->size = 0xdead; // c1->size != prev_size (p2)

    free(p2); // corrupted size vs. prev_size while consolidating
    return 0;
}