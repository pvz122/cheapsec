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
    //       if (victim != NULL)
    // {
    //   if (__glibc_unlikely (misaligned_chunk (victim)))
    //     malloc_printerr ("malloc(): unaligned fastbin chunk detected 2");

    // ---- get arena struct address ----
    void *us = malloc(0x410); // unsorted bin
    void *br = malloc(0x410); // barrier

    free(us);

    //   ...
    //   /* Fastbins */
    //   mfastbinptr fastbinsY[NFASTBINS];
    //   /* Base of the topmost chunk -- not otherwise kept in a bin */
    //   mchunkptr top;
    //   /* The remainder from the most recent split of a small request */
    //   mchunkptr last_remainder;
    //   /* Normal bins packed as described above */
    //   mchunkptr bins[NBINS * 2 - 2];
    //   /* Bitmap of bins */
    //   unsigned int binmap[BINMAPSIZE];
    //   ...

    chunk_p cus = (chunk_p)((char *)us - 0x10);

    chunk_p *top = (chunk_p *)cus->bk;
    chunk_p *fastbinsY = (chunk_p *)((void *)cus->bk - (0x8 * 10));
    chunk_p *bins = (chunk_p *)((void *)cus->bk + 0x10);

    free(br);
    // ---- get arena struct address ----

    // target fastbin chunks
    void *p1 = malloc(0x20);

    // fill up the tcache
    void *fill[7];
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x20);
    }
    for (int i = 0; i < 7; i++) {
        free(fill[i]);
    }

    free(p1);

    // fastbin -> p1

    fastbinsY[1] = (chunk_p)((char *)p1 - 0x9); // fastbin -> unaligned chunk

    // clear the tcache
    for (int i = 0; i < 7; i++) {
        fill[i] = malloc(0x20);
    }

    malloc(0x20); // malloc(): unaligned fastbin chunk detected 2
    return 0;
}