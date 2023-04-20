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
    //   for (;; )
    //     {
    //       int iters = 0;
    //       while ((victim = unsorted_chunks (av)->bk) != unsorted_chunks (av))
    //         {
    //           bck = victim->bk;
    //           size = chunksize (victim);
    //           mchunkptr next = chunk_at_offset (victim, size);

    //           ...
    //           if (__glibc_unlikely (prev_inuse (next)))
    //             malloc_printerr ("malloc(): invalid next->prev_inuse (unsorted)");

    // target unsorted chunk
    void *p1 = malloc(0x800);
    void *barrier = malloc(0x20);

    // put p1 into unsorted bin
    free(p1);

    // unsorted bin -> p1

    chunk_p c_barrier = (chunk_p)((char *)barrier - 0x10);
    c_barrier->size |= 0x1; // next->prev_inuse != 0

    malloc(0x800); // malloc(): invalid next->prev_inuse (unsorted)
    return 0;
}