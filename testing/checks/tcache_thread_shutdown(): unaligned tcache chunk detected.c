#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

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

void *work(void *arg)
{
    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);

    free(p1);
    free(p2);

    // tache -> p2 -> p1

    char buffer[100] = { 0 };
    chunk_p fake = (chunk_p)&buffer[1]; // make it unaligned

    chunk_p c2 = (chunk_p)((char *)p2 - 0x10);
    c2->fd = fake; // link fake to p2

    return NULL;  // tcache_thread_shutdown(): unaligned tcache chunk detected
}

int main(void)
{
    //   /* Free all of the entries and the tcache itself back to the arena
    //      heap for coalescing.  */
    //   for (i = 0; i < TCACHE_MAX_BINS; ++i)
    //     {
    //       while (tcache_tmp->entries[i])
    // 	{
    // 	  tcache_entry *e = tcache_tmp->entries[i];
    // 	  if (__glibc_unlikely (!aligned_OK (e)))
    // 	    malloc_printerr ("tcache_thread_shutdown(): "
    // 			     "unaligned tcache chunk detected");
    // 	  tcache_tmp->entries[i] = REVEAL_PTR (e->next);
    // 	  __libc_free (e);
    // 	}
    //     }

    pthread_t t;
    pthread_create(&t, NULL, work, NULL);
    pthread_join(t, NULL);

    return 0;
}