#ifndef __C_POLICY__
#define __C_POLICY__

#include "hand.h"
#include "game.h"
#include "policy.h"

#include "c_cribbage.h"

namespace cpsc474
{
  class CPolicy : public CribbagePolicy
  {
  public:
  CPolicy(const CribbageGame& game, KEEP_POLICY k, PEG_POLICY p) : game(game), cKeep(k), cPeg(p) {}
    
    std::vector<CribbageHand *> keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer);

    const CribbageCard *peg(const CribbageHand& cards, const PeggingHistory& hist, const std::vector<int>& scores, bool amDealer);

  private:
    const CribbageGame& game;
    KEEP_POLICY cKeep;
    PEG_POLICY cPeg;
  };
}

#endif
