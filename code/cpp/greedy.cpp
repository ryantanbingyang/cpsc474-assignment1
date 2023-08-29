#include "greedy.h"

#include <vector>

#include <climits>

#include "game.h"
#include "hand.h"
#include "policy.h"
#include "utility.h"

#include "c_utility.h"

namespace cpsc474
{
  
  std::vector<CribbageHand *> GreedyThrower::keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer)
  {
    // iterate over all possible actions, randomly uniformly choosing among
    // those that maximize the net score
    
    std::vector<CribbageHand *> bestSplit;
    int bestNet = INT_MIN;
    size_t ties = 0;
    const std::vector<std::vector<size_t>>& allThrows = game.getThrows();
    for (const auto& throwIndices : allThrows)
      {
	std::vector<CribbageHand *> split = cards.split(throwIndices);
	int netPoints = game.score(*split[0], nullptr, false)[0] + (amDealer ? 1 : -1) * game.score(*split[1], nullptr, true)[0];
	if (netPoints > bestNet)
	  {
	    for (CribbageHand *h : bestSplit)
	      {
		delete h;
	      }
	    bestSplit = split;
	    bestNet = netPoints;
	    ties = 0;
	  }
	else if (netPoints == bestNet && ties++ >= 0 && random01() < 1.0 / ties)
	  {
	    for (CribbageHand *h : bestSplit)
	      {
		delete h;
	      }

	    bestSplit = split;
	  }
	else
	  {
	    for (CribbageHand *hand : split)
	      {
		delete hand;
	      }
	  }
	
      }

    return bestSplit;
  }

  const CribbageCard *GreedyPegger::peg(const CribbageHand& cards, const PeggingHistory& hist, const CribbageCard& turn, const std::vector<int>& scores, bool amDealer)
  {
    const CribbageCard *selected = nullptr;
    size_t ties = 0;
    int bestScore = INT_MIN;
    for (const auto& c : cards.cards())
      {
	std::vector<int> cardScore = hist.score(c, amDealer ? 0 : 1);
	if (cardScore.size() > 0)
	  {
	    // card is a valid play
	    if (cardScore[0] > bestScore)
	      {
		bestScore = cardScore[0];
		selected = c;
		ties = 0;
	      }
	    else if (cardScore[0] == bestScore)
	      {
		ties++;
		
		if (random01() < 1.0 / ties)
		  {
		    // effect is to uniformly randomly select from tied cards
		    selected = c;
		  }
	      }
	    
	  }
      }
    return selected;
  }

}
