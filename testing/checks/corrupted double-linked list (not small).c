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
    //   if (!in_smallbin_range (chunksize_nomask (p)) && p->fd_nextsize != NULL)
    //     {
    //       if (p->fd_nextsize->bk_nextsize != p
    // 	  || p->bk_nextsize->fd_nextsize != p)
    // 	malloc_printerr ("corrupted double-linked list (not small)");

    void *barrier1 = malloc(0x410);
    void *p1 = malloc(0x440); // largebin chunk
    void *barrier2 = malloc(0x410);

    free(p1);
    malloc(0x810); // put p1 into largebin

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    c1->fd_nextsize = (chunk_p)barrier1; // corrupted fd_nextsize

    malloc(0x440); // unlink p1

    return 0;
}