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
    //       /* Little security check which won't hurt performance: the
    //      allocator never wrapps around at the end of the address space.
    //      Therefore we can exclude some size values which might appear
    //      here by accident or by "design" from some intruder.  */
    //   if (__builtin_expect ((uintptr_t) p > (uintptr_t) -size, 0)
    //       || __builtin_expect (misaligned_chunk (p), 0))
    //     malloc_printerr ("free(): invalid pointer");

    void *p = malloc(0x100);

    free(p + 0x1); // free(): invalid pointer
    return 0;
}