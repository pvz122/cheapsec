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
    //     /* Atomically link P to its fastbin: P->FD = *FB; *FB = P;  */
    // mchunkptr old = *fb, old2;

    // if (SINGLE_THREAD_P)
    //   {
    // /* Check that the top of the bin is not the record we are going to
    //    add (i.e., double free).  */
    // if (__builtin_expect (old == p, 0))
    //   malloc_printerr ("double free or corruption (fasttop)");

    void *p = malloc(0x20);

    // fill tcache
    void *fill[7];
    for (int i = 0; i < 7; i++)
        fill[i] = malloc(0x20);
    for (int i = 0; i < 7; i++)
        free(fill[i]);

    free(p);
    free(p); // double free or corruption (fasttop)
    return 0;
}