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
    //       next = chunk_at_offset (oldp, oldsize);
    //   INTERNAL_SIZE_T nextsize = chunksize (next);
    //   if (__builtin_expect (chunksize_nomask (next) <= CHUNK_HDR_SZ, 0)
    //       || __builtin_expect (nextsize >= av->system_mem, 0))
    //       malloc_printerr("realloc(): invalid next size");

    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);

    chunk_p c2 = (chunk_p)((char *)p2 - 0x10);
    c2->size = 0x8 | 0x1; // c2->size <= CHUNK_HDR_SZ

    p1 = realloc(p1, 0x20); // realloc(): invalid next size
    return 0;
}