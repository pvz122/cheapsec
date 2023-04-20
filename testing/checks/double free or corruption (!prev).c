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
    // /* Or whether the block is actually not marked used.  */
    // if (__glibc_unlikely (!prev_inuse(nextchunk)))
    //   malloc_printerr ("double free or corruption (!prev)");

    void *p = malloc(0x410);
    void *barrier = malloc(0x20);

    free(p);
    free(p); // double free or corruption (!prev)
    return 0;
}