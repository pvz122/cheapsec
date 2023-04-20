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
    //       uintptr_t mem = (uintptr_t) chunk2mem (p);
    //   uintptr_t block = (uintptr_t) p - prev_size (p);
    //   size_t total_size = prev_size (p) + size;
    //   /* Unfortunately we have to do the compilers job by hand here.  Normally
    //      we would test BLOCK and TOTAL-SIZE separately for compliance with the
    //      page size.  But gcc does not recognize the optimization possibility
    //      (in the moment at least) so we combine the two values into one before
    //      the bit test.  */
    //   if (__glibc_unlikely ((block | total_size) & (pagesize - 1)) != 0
    //       || __glibc_unlikely (!powerof2 (mem & (pagesize - 1))))
    //     malloc_printerr ("munmap_chunk(): invalid pointer");

    void *p = malloc(0x1000000); // 16MB
    chunk_p chunk = (chunk_p)((char *)p - 0x10);

    chunk->prev_size = 0xdead; // invalid prev_size

    free(p); // munmap_chunk(): invalid pointer

    return 0;
}