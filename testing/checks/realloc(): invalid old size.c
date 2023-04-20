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
    //   /* oldmem size */
    //   if (__builtin_expect (chunksize_nomask (oldp) <= CHUNK_HDR_SZ, 0)
    //       || __builtin_expect (oldsize >= av->system_mem, 0)
    //       || __builtin_expect (oldsize != chunksize (oldp), 0))
    //     malloc_printerr ("realloc(): invalid old size");

    void *p = malloc(0x20);

    chunk_p c = (chunk_p)((char *)p - 0x10);
    c->size = 0x8 | 0x1; // c->size <= CHUNK_HDR_SZ

    p = realloc(p, 0x20); // realloc(): invalid old size
    return 0;
}