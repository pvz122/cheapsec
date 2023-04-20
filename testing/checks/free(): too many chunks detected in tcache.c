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
    // 	/* This test succeeds on double free.  However, we don't 100%
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
    // 	if (cnt >= mp_.tcache_count)
    // 	  malloc_printerr ("free(): too many chunks detected in tcache");

    // target tcache chunk
    void *p1 = malloc(0x20);
    chunk_p c1 = (chunk_p)((char *)p1 - 0x10);

    // put it into tcache
    free(p1);
    // tcache -> c1 -> NULL

    // fill tcache
    void *fill[7];
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x20);
    }
    for (int i = 0; i < 7; i++) {
        free(fill[i]);
    }
    // tcache -> ... -> c1 -> NULL

    chunk_p fake = malloc(0x30);
    fake->size = 0x30 | 0x1;
    fake->fd = NULL;
    fake->bk = c1->bk; // fake->tcache_key = c1->tcache_key, double free detected

    c1->fd = fake; // link c1 to fake
    // tcache -> ... -> c1 -> fake -> NULL

    free(&fake->fd); // free(): too many chunks detected in tcache

    return 0;
}