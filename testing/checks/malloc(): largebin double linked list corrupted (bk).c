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
    //       bck = fwd->bk;
    //   if (bck->fd != fwd)
    //     malloc_printerr ("malloc(): largebin double linked list corrupted (bk)");

            // target largebin chunks
    void *p1 = malloc(0x500);
    void *barrier1 = malloc(0x20);
    void *p2 = malloc(0x510);
    void *barrier2 = malloc(0x20);

    // put p1 into largebin
    free(p1);
    malloc(0x800);

    // largebin -> p1

    chunk_t fake = { 0 };
    fake.fd = (chunk_p)0xdeadbeef;

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->bk = &fake; // c1->bk->fd != c1

    // put p2 into largebin
    free(p2);
    malloc(0x800); // malloc(): largebin double linked list corrupted (bk)
    return 0;
}