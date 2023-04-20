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
    //   /* remove from unsorted list */
    //   if (__glibc_unlikely (bck->fd != victim))
    //     malloc_printerr ("malloc(): corrupted unsorted chunks 3");
    //   unsorted_chunks (av)->bk = bck;
    //   bck->fd = unsorted_chunks (av);

    // target unsorted chunk
    void *p1 = malloc(0x800);
    void *barrier = malloc(0x20);

    // put p1 into unsorted bin
    free(p1);

    // unsorted bin -> p1

    chunk_t fake = { 0 };
    fake.fd = (chunk_p)0xdeadbeef;

    chunk_p c = (chunk_p)((char *)p1 - 0x10);
    c->bk = &fake; // c->bk->fd != victim

    malloc(0x800); // malloc(): corrupted unsorted chunks 3
    return 0;
}