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
    //         /* Check that size of fastbin chunk at the top is the same as
    //        size of the chunk that we are adding.  We can dereference OLD
    //        only if we have the lock, otherwise it might have already been
    //        allocated again.  */
    //     if (have_lock && old != NULL
    //   && __builtin_expect (fastbin_index (chunksize (old)) != idx, 0))
    //       malloc_printerr ("invalid fastbin entry (free)");
    //   }

    // target fastbin chunks
    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);

    // fill tcache
    void *fill[7];
    for (int i = 0; i < 7; i++)
        fill[i] = malloc(0x20);
    for (int i = 0; i < 7; i++)
        free(fill[i]);

    // put p1 into fastbin
    free(p1);

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->size = 0x81; // invalid size, original size is 0x31

    p2 = realloc(p2, 0x40); // invalid fastbin entry

    return 0;
}