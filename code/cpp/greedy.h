#ifndef __GREEDY_H__
#define __GREEDY_H__

#include <vector>

#include "hand.h"
#include "game.h"
#include "policy.h"

namespace cpsc474
{

  class GreedyThrower : public KeepPolicy
  {
  public:
  GreedyThrower(const CribbageGame& game) : game(game) {}
    
    std::vector<CribbageHand *> keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer);
    
  protected:
    const CribbageGame& game;
  };

  class GreedyPegger : public PegPolicy
  {
  public:
  GreedyPegger(const CribbageGame& game) : game(game) {}

    const CribbageCard *peg(const CribbageHand& cards, const PeggingHistory& hist, const CribbageCard& turn, const std::vector<int>& scores, bool amDealer);

  protected:
    const CribbageGame& game;
  };

  
}

#endif
