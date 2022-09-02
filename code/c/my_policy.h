#ifndef __MY_POLICY_H__
#define __MY_POLICY_H__

#include <vector>

#include "hand.h"
#include "game.h"
#include "policy.h"

namespace cpsc474
{

  class MyPolicy : public CribbagePolicy
  {
  public:
    MyPolicy(const CribbageGame& game);

    ~MyPolicy();
    
    std::vector<CribbageHand *> keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer);

    const CribbageCard *peg(const CribbageHand& cards,const PeggingHistory& hist, const std::vector<int>& scores, bool amDealer);

  protected:
    KeepPolicy *keepPolicy;
    PegPolicy *pegPolicy;
    CribbagePolicy *policy;
  };
  
}

#endif
