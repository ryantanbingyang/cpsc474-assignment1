#ifndef __C_GREEDY_H__
#define __C_GREEDY_H__

#ifdef __cplusplus
extern "C"
{
#endif

  void greedy_keep(const void *game, const void *hand, int scores[2], int am_dealer, void *split[2]);

  const void *greedy_peg(const void *game, const void *hand, const void *hist, const void *turn, int scores[2], int am_dealer);


#ifdef __cplusplus
}
#endif
  
#endif

