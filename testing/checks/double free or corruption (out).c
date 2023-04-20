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
    //     /* Or whether the next chunk is beyond the boundaries of the arena.  */
    // if (__builtin_expect (contiguous (av)
    // 		  && (char *) nextchunk
    // 		  >= ((char *) av->top + chunksize(av->top)), 0))
    // malloc_printerr ("double free or corruption (out)");

    void *p1 = malloc(0x410);

    void *p2 = malloc(0x410);
    free(p2);
    // now p2 = av->top

    free(p1);
    // now p2 != av->top, but it is contained in the top chunk

    free(p2); // double free or corruption (out)
    return 0;
}