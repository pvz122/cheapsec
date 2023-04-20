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
    //   if (chunksize (p) != prev_size (next_chunk (p)))
    //     malloc_printerr ("corrupted size vs. prev_size");

    void *p1 = malloc(0x410);
    void *p2 = malloc(0x410);
    void *p3 = malloc(0x410);
    void *p4 = malloc(0x410);

    // ^ top
    // p4
    // p3
    // p2
    // p1

    free(p3);

    chunk_p c3 = (chunk_p)((char *)p3 - 0x10);
    c3->size = 0x430 | 0x1; // corrupted size

    free(p2); // forward consolidation, unlinking p3
    return 0;
}