#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

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
    sleep(1000);
    return NULL;
}

int main(void)
{
    //     #define REMOVE_FB(fb, victim, pp)			\
    //   do							\
    //     {							\
    //       victim = pp;					\
    //       if (victim == NULL)				\
    // 	break;						\
    //       pp = REVEAL_PTR (victim->fd);                                     \
    //       if (__glibc_unlikely (pp != NULL && misaligned_chunk (pp)))       \
    // 	malloc_printerr ("malloc(): unaligned fastbin chunk detected"); \
    //     }							\
    //   while ((pp = catomic_compare_and_exchange_val_acq (fb, pp, victim)) \
    // 	 != victim);		

    // --- trigger ---
    // 	  if (SINGLE_THREAD_P)
    //     *fb = REVEAL_PTR (victim->fd);
    //   else
    //     REMOVE_FB (fb, pp, victim);
    // --- REMOVE_FB only works for non-single thread ---

    // enable multi-thread
    pthread_t t;
    pthread_create(&t, NULL, work, NULL);

    // target fastbin chunks
    void *p1 = malloc(0x20);
    void *p2 = malloc(0x20);

    // fill up the tcache
    void *fill[7];
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x20);
    }
    for (int i = 0; i < 7; i++) {
        free(fill[i]);
    }

    free(p1);
    free(p2);

    // fastbin -> p2 -> p1

    chunk_p c2 = (chunk_p)((char *)p2 - 0x10);
    c2->fd = (chunk_p)((char *)p1 - 0x9); // make p2->fd unaligned

    // clear the tcache
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x20);
    }

    malloc(0x20); // malloc(): unaligned fastbin chunk detected
    return 0;
}