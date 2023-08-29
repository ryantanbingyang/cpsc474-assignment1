#include "policy.h"

#include <vector>

#include "hand.h"
#include "history.h"
#include "game.h"
#include "utility.h"

#include "c_utility.h"

namespace cpsc474
{
  
  std::vector<CribbageHand *> RandomThrower::keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer)
  {
	// select two indices uniformly randomly
	size_t index1 = random() % cards.size();
	size_t index2 = random() % (cards.size() - 1);
	if (index2 >= index1)
	    {
		index2++;
	    }

	// sort the indices
	if (index2 < index1)
	  {
	    size_t temp = index1;
	    index1 = index2;
	    index2 = temp;
	  }

	// split the hand
	return cards.split(std::vector<size_t> {index1, index2});
  }

  const CribbageCard *RandomPegger::peg(const CribbageHand& cards, const PeggingHistory& hist, const CribbageCard& turn, const std::vector<int>& scores, bool amDealer)
  {
    const CribbageCard *selected = nullptr;
    size_t validCards = 0;
    for (const CribbageCard *c : cards.cards())
      {
	std::vector<int> cardScore = hist.score(c, amDealer ? 0 : 1);
	if (cardScore.size() > 0)
	  {
	    // card is a valid play
	    validCards++;
	    if (random01() < 1.0 / validCards)
	      {
		selected = c;
	      }
	  }
      }
    return selected;
  }

}
