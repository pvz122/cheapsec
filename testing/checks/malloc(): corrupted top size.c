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
    //   use_top:
    // victim = av->top;
    // size = chunksize (victim);

    // if (__glibc_unlikely (size > av->system_mem))
    //   malloc_printerr ("malloc(): corrupted top size");

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

    (*top)->size = -1; // top->size > av->system_mem

    malloc(0x100); // malloc(): corrupted top size
    return 0;
}