#include "my_policy.h"

#include <vector>

#include <climits>

#include "game.h"
#include "hand.h"
#include "policy.h"
#include "greedy.h"

namespace cpsc474
{
  MyPolicy::MyPolicy(const CribbageGame& game)
    : keepPolicy(new GreedyThrower(game)),
      pegPolicy(new GreedyPegger(game)),
      policy(new CompoundPolicy(*keepPolicy, *pegPolicy))
  {
  }

  MyPolicy::~MyPolicy()
  {
    delete policy;
    delete keepPolicy;
    delete pegPolicy;
  }
  
  std::vector<CribbageHand *> MyPolicy::keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer)
  {
    return policy->keep(cards, scores, amDealer);
  }

  const CribbageCard *MyPolicy::peg(const CribbageHand& cards, const PeggingHistory& hist, const std::vector<int>& scores, bool amDealer)
  {
    return policy->peg(cards, hist, scores, amDealer);
  }

}
