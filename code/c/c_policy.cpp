#include "c_policy.h"

#include <vector>

#include "hand.h"
#include "game.h"

namespace cpsc474
{
  std::vector<CribbageHand *> CPolicy::keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer)
  {
    int s[2] = { scores[0], scores[1] };
    void *split[2];
    
    cKeep(&game, &cards, s, amDealer, split);

    return std::vector<CribbageHand *> { (CribbageHand *)split[0], (CribbageHand *)split[1] };
  }

  const CribbageCard *CPolicy::peg(const CribbageHand& cards, const PeggingHistory& hist, const std::vector<int>& scores, bool amDealer)
  {
    int s[2] = { scores[0], scores[1] };

    return (const CribbageCard *)cPeg(&game, &cards, &hist, s, amDealer);
  }

}
