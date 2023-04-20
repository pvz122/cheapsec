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
    //     nextsize = chunksize(nextchunk);
    // if (__builtin_expect (chunksize_nomask (nextchunk) <= CHUNK_HDR_SZ, 0)
    // || __builtin_expect (nextsize >= av->system_mem, 0))
    //   malloc_printerr ("free(): invalid next size (normal)");

    // target unsorted bin chunks
    void *p1 = malloc(0x410);
    void *p2 = malloc(0x410);

    chunk_p c2 = (chunk_p)((char *)p2 - 0x10);
    c2->size = -1; // c2->size > av->system_mem

    free(p1); // free(): invalid next size (normal)
    return 0;
}