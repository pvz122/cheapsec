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
    //       idx = fastbin_index (nb);

    // 		size_t victim_idx = fastbin_index (chunksize (victim));
    //   if (__builtin_expect (victim_idx != idx, 0))
    // malloc_printerr ("malloc(): memory corruption (fast)");

    // target fastbin chunks
    void *p1 = malloc(0x20);

    // fill up the tcache
    void *fill[7];
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x20);
    }
    for (int i = 0; i < 7; i++) {
        free(fill[i]);
    }

    free(p1);

    // fastbin -> p1

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->size = 0x21; // invalid size, which originally is 0x31

    // clear the tcache
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x20);
    }

    malloc(0x20); // malloc(): memory corruption (fast)
    return 0;
}