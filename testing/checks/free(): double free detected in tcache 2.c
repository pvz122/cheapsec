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
    // 	...
    // 	if (tmp == e)
    // 	  malloc_printerr ("free(): double free detected in tcache 2");

    void *p = malloc(0x20);

    free(p);
    free(p); // free(): double free detected in tcache 2
    return 0;
}