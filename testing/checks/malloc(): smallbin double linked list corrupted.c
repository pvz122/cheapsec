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
    //   if (in_smallbin_range (nb))
    // {
    //   idx = smallbin_index (nb);
    //   bin = bin_at (av, idx);

    //   if ((victim = last (bin)) != bin)
    //     {
    //       bck = victim->bk;
    //   if (__glibc_unlikely (bck->fd != victim))
    //     malloc_printerr ("malloc(): smallbin double linked list corrupted");

    // target smallbin chunks
    void *p1 = malloc(0x100);

    // fill up the tcache
    void *fill[7];
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x100);
    }
    for (int i = 0; i < 7; i++) {
        free(fill[i]);
    }

    // put p1 into smallbin
    free(p1);
    malloc(0x800);

    // smallbin -> p1
    //          <-

    chunk_t fake = { 0 };
    fake.fd = NULL;

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->bk = &fake; // c1->bk->fd = NULL != c1

    // clear the tcache
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x100);
    }

    malloc(0x100); // malloc(): smallbin double linked list corrupted
    return 0;
}