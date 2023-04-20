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
    //     /* Caller must ensure that we know tc_idx is valid and there's
    //    available chunks to remove.  */
    // static __always_inline void *
    // tcache_get (size_t tc_idx)
    // {
    //   tcache_entry *e = tcache->entries[tc_idx];
    //   if (__glibc_unlikely (!aligned_OK (e)))
    //     malloc_printerr ("malloc(): unaligned tcache chunk detected");
    //   tcache->entries[tc_idx] = REVEAL_PTR (e->next);
    //   --(tcache->counts[tc_idx]);
    //   e->key = 0;
    //   return (void *) e;
    // }

    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);

    free(p1);
    free(p2);

    // tache -> p2 -> p1

    chunk_t fake = { 0 };

    chunk_p c2 = (chunk_p)((char *)p2 - 0x10);
    c2->fd = &fake; // link fake to p2

    malloc(0x20);
    malloc(0x20); // malloc(): unaligned tcache chunk detected
    return 0;
}