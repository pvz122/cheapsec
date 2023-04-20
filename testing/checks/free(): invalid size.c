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
    //   /* We know that each chunk is at least MINSIZE bytes in size or a
    //      multiple of MALLOC_ALIGNMENT.  */
    //   if (__glibc_unlikely (size < MINSIZE || !aligned_OK (size)))
    //     malloc_printerr ("free(): invalid size");

    void *p = malloc(0x100);

    chunk_p c = (chunk_p)((char *)p - 0x10);
    c->size = 0x8 | 0x1; // c->size < MINSIZE

    free(p); // free(): invalid size
    return 0;
}