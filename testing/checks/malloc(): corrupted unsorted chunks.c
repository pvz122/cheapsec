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
    //     /* Split */
    //     else
    //       {
    //         remainder = chunk_at_offset (victim, nb);
    //         /* We cannot assume the unsorted list is empty and therefore
    //            have to perform a complete insert here.  */
    //         bck = unsorted_chunks (av);
    //         fwd = bck->fd;
    // if (__glibc_unlikely (fwd->bk != bck))
    // malloc_printerr ("malloc(): corrupted unsorted chunks");
    //         remainder->bk = bck;
    //         remainder->fd = fwd;
    //         bck->fd = remainder;
    //         fwd->bk = remainder;


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

    // target large bin chunk
    void *p1 = malloc(0x520);
    void *barrier = malloc(0x20);

    // put p1 into large bin
    free(p1);
    malloc(0x800);

    // large bin -> p1
    // unsorted bin -> NULL

    chunk_t fake = { 0 };

    bins[0] = &fake; // unsorted bin -> fake
    fake.bk = (chunk_p)0xdeadbeef; // unsorted bin head->fd->bk = 0xdeadbeef != unsorted bin head

    malloc(0x500); // malloc(): corrupted unsorted chunks

    return 0;
}