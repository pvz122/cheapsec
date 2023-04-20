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
    //       /* chunk corresponding to oldmem */
    //   const mchunkptr oldp = mem2chunk (oldmem);
    //   /* its size */
    //   const INTERNAL_SIZE_T oldsize = chunksize (oldp);

    //   /* Little security check which won't hurt performance: the allocator
    //      never wrapps around at the end of the address space.  Therefore
    //      we can exclude some size values which might appear here by
    //      accident or by "design" from some intruder.  */
    //   if ((__builtin_expect ((uintptr_t) oldp > (uintptr_t) -oldsize, 0)
    //        || __builtin_expect (misaligned_chunk (oldp), 0)))
    //       malloc_printerr ("realloc(): invalid pointer");

    void *p1 = malloc(0x20);

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->size = -1; // invalid size

    p1 = realloc(p1, 0x40); // realloc(): invalid pointer

    return 0;
}