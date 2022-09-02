#include "c_cribbage.h"

#include "c_greedy.h"

void keep(const void *game, const void *hand, int scores[2], int am_dealer, void *split[2])
{
  return greedy_keep(game, hand, scores, am_dealer, split);
}

const void *peg(const void *game, const void *hand, const void *hist, int scores[2], int am_dealer)
{
  return greedy_peg(game, hand, hist, scores, am_dealer);
}
