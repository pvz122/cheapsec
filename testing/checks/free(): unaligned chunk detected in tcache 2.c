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
    // /* This test succeeds on double free.  However, we don't 100%
    //    trust it (it also matches random payload data at a 1 in
    //    2^<size_t> chance), so verify it's not an unlikely
    //    coincidence before aborting.  */
    // if (__glibc_unlikely (e->key == tcache_key))
    //   {
    //     tcache_entry *tmp;
    //     size_t cnt = 0;
    //     LIBC_PROBE (memory_tcache_double_free, 2, e, tc_idx);
    //     for (tmp = tcache->entries[tc_idx];
    // 	 tmp;
    // 	 tmp = REVEAL_PTR (tmp->next), ++cnt)
    //       {
    //   ...
    // 	if (__glibc_unlikely (!aligned_OK (tmp)))
    // 	  malloc_printerr ("free(): unaligned chunk detected in tcache 2");

    // target tcache chunk
    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);
    void *p3 = malloc(0x30);

    // put it into tcache
    free(p1);
    // tcache -> c1 -> NULL

    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);
    chunk_p c2 = (chunk_p)((char *)p2 - 0x10);
    c2->bk = c1->bk; // c2->tcache_key = c1->tcache_key, double free detected

    chunk_p fake = p3 + 0x1; // unaligned fake chunk
    fake->size = 0x30 | 0x1;
    fake->fd = NULL;

    c1->fd = fake; // link fake to tcache
    // tcache -> c1 -> fake -> NULL

    free(p2); // free(): unaligned chunk detected in tcache 2

    return 0;
}