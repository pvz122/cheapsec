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
    // if (!have_lock)
    //   {
    //     __libc_lock_lock (av->mutex);
    //     fail = (chunksize_nomask (chunk_at_offset (p, size)) <= CHUNK_HDR_SZ
    // 	    || chunksize (chunk_at_offset (p, size)) >= av->system_mem);
    //     __libc_lock_unlock (av->mutex);
    //   }

    // if (fail)
    //   malloc_printerr ("free(): invalid next size (fast)");
    //   }

    // target fastbin chunk
    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);

    // fill tcache
    void *fill[7];
    for (int i = 0; i < 7; i++)
        fill[i] = malloc(0x20);
    for (int i = 0; i < 7; i++)
        free(fill[i]);

    chunk_p c2 = (chunk_p)((char *)p2 - 0x10);
    c2->size = -1; // c2->size > av->system_mem

    free(p1); // free(): invalid next size (fast)

    return 0;
}