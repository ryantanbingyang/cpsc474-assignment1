#include "history.h"

#include <numeric>

#include <utility>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include <stdlib.h>

#include "card.h"
#include "game.h"

namespace cpsc474
{
  std::vector<int> zeroScore(5, 0);
  std::vector<bool> noPasses(2, false);
  std::vector<size_t> zeroPlayed(2, 0);
  
  PeggingHistory::PeggingHistory(const CribbageGame& game, const PeggingHistory *prevPlay, const PeggingHistory *prevRound, int total, const std::vector<bool>& passed, const std::vector<size_t>& played, const CribbageCard *card, int player, const std::vector<int>& score)
    : game(game), prevPlay(prevPlay), prevRound(prevRound), card(card), player(player), lastScore(score), total(total), passed(passed), cardsPlayed(played)
  {
  }
  
  bool PeggingHistory::isTerminal() const
  {
    return std::accumulate(cardsPlayed.begin(), cardsPlayed.end(), (size_t)0) == game.cardsToKeep() * 2;
  }

  const std::vector<int>& PeggingHistory::getScore() const
  {
    return lastScore;
  }

  int PeggingHistory::getTotal() const
  {
    return total;
  }

  bool PeggingHistory::startRound() const
  {
    return total == 0 || total == game.getPeggingLimit() || (passed[0] && passed[1]);
  }

  bool PeggingHistory::hasPassed(int player) const
  {
    return passed[player];
  }

  PeggingHistory *PeggingHistory::play(const CribbageCard *card, int player) const
  {
    if (this->player != -1 && player != 1 - this->player)
      {
	// wrong player
	return nullptr;
      }
    
    std::vector<int> nextScore = score(card, player);
    
    if (nextScore.size() == 0)
      {
	// illegal card
	return nullptr;
      }
    
    int nextTotal = startRound() ? 0 : total;
    if (card != nullptr)
      {
	nextTotal += game.rankValue(card->getRank());
      }
    if (nextTotal > game.getPeggingLimit())
      {
	return nullptr;
      }

    // update cards played
    std::vector<size_t> nextCardsPlayed = cardsPlayed;
    if (card != nullptr)
      {
	nextCardsPlayed[player] += 1;
      }
    
    // updated passes
    std::vector<bool> nextPassed = startRound() ? std::vector<bool>(2, false) : passed;
    if (card == nullptr)
      {
	nextPassed[player] = true;
      }

    const PeggingHistory *nextPrevRound, *nextPrevPlay;
    if (startRound())
      {
	nextPrevPlay = nullptr;
	nextPrevRound = this;
      }
    else
      {
	nextPrevPlay = this;
	nextPrevRound = this->prevRound;
      }
    
    return new PeggingHistory(game, nextPrevPlay, nextPrevRound, nextTotal, nextPassed, nextCardsPlayed, card, player, nextScore);
  }

  bool PeggingHistory::isLegal(const CribbageCard& card, int player) const
  {
    return (startRound() ? 0 : total) + game.rankValue(card.getRank()) <= game.getPeggingLimit();
  }

  bool PeggingHistory::hasLegalPlay(const CribbageHand& hand, int player) const
  {
    if (passed[player])
      {
	return false;
      }
    for (const CribbageCard *c : hand.cards())
      {
	if (isLegal(*c, player))
	  {
	    return true;
	  }
      }
	   
    return false;
  }

  std::vector<int> PeggingHistory::score(const CribbageCard *card, int player) const
  {
    if (card == nullptr)
      {
	if (passed[player])
	  {
	    // player has already passed
	    int result[] = {0, 0, 0, 0, 0};
	    return std::vector<int>(result, result + 5);
	  }
	else if (!passed[1 - player])
	  {
	    // "go"
	    // player has already passed
	    int result[] = {-1, 0, 0, 0, -1};
	    return std::vector<int>(result, result + 5);
	  }
	else
	  {
	    // other player already conceded "go"
	    return std::vector<int>(5, 0);
	  }
      }
    else if (!startRound() && passed[player])
      {
	// player has already passed but is playing a card
	return std::vector<int>();
      }
    
    // reset total at start of a round
    int prevTotal = startRound() ? 0 : total;
    
    if (prevTotal + game.rankValue(card->getRank()) > game.getPeggingLimit())
      {
	// card puts count over 31
	return std::vector<int>();
      }
    
    // assert(card != null)
    
    // initialized tracking variables for card being played
    int countPlayed = 1; // number of cards examined (starting with param)
    int currMatches = 1; // number of consecutive cards that match
    int maxMatches = 1; // maximum number of matches seen
    int maxStraight = 1; // maximum run seen
    const CardRank *minRank = &card->getRank(); // min rank seen
    const CardRank *maxRank = &card->getRank(); // max rank seen
    bool doubles = false; // whether a duplicate has been seen (breaking a run)
    std::unordered_set<int> ranksSeen;
    ranksSeen.insert(card->getRank().intValue());
    
    // scan history backwards to determine runs and pairs starting with
    // the current step
    const PeggingHistory *curr = startRound() ? nullptr : this;
    // stop at beginning of round or when run not possible and not still
    // matching rank
    while (curr != nullptr && (countPlayed == 0 || currMatches == maxMatches || !doubles))
      {
	if (curr->card != nullptr)
	  {
	    countPlayed += 1;
	    
	    if (card->getRank() == curr->card->getRank())
	      {
		// ranks match
		if (currMatches != -1)
		  {
		    currMatches += 1;
		  }
		maxMatches = currMatches > maxMatches ? currMatches : maxMatches;
	      }
	    else
	      {
		// ranks don't match -- ignore for rest of history
		currMatches = -1;
	      }
	    
	    // update min/max rank
	    if (curr->card->getRank().intValue() < minRank->intValue())
	      {
		minRank = &curr->card->getRank();
	      }
	    else if (curr->card->getRank().intValue() > maxRank->intValue())
	      {
		maxRank = &curr->card->getRank();
	      }
	    
	    // update ranks/duplicates seen
	    if (ranksSeen.count(curr->card->getRank().intValue()) > 0)
	      {
		doubles = true;
	      }
	    else
	      {
		ranksSeen.insert(curr->card->getRank().intValue());
	      }
	    
	    if (!doubles && maxRank->intValue() - minRank->intValue() + 1 == countPlayed)
	      {
		// no duplicates, max-min+1 == count -> run
		maxStraight = countPlayed;
	      }
	    
	  }
	
	curr = curr->prevPlay;
      }
    
    int pairScore = game.pegPairValue(maxMatches);
    int straightScore = game.pegStraightValue(maxStraight);
    int fifteenScore = game.pegSumValue(prevTotal + game.rankValue(card->getRank()));
    int lastScore = 0;
    if (prevTotal + game.rankValue(card->getRank()) == game.getPeggingLimit())
      {
	lastScore = game.pegExactValue(passed[1 - player]);
      }
    else if (std::accumulate(cardsPlayed.begin(), cardsPlayed.end(), (size_t)0) + 1 == 2 * game.cardsToKeep())
      {
	// eighth card played
	lastScore = 1;
      }
    
    std::vector<int> result;
    result.push_back(pairScore + fifteenScore + straightScore + lastScore);
    result.push_back(pairScore);
    result.push_back(fifteenScore);
    result.push_back(straightScore);
    result.push_back(lastScore);
    return result;
  }

  std::vector<std::vector<std::pair<int, const CribbageCard *>>> PeggingHistory::plays() const
  {
    std::vector<std::vector<std::pair<int, const CribbageCard *>>> result;
    std::vector<std::pair<int, const CribbageCard *>> round;
    const PeggingHistory *curr = this;
    while (curr != nullptr)
      {
	if (curr->player != -1)
	  {
	    round.push_back(std::make_pair(curr->player, curr->card));
	  }
	if (curr->prevPlay == nullptr)
	  {
	    if (round.size() > 0)
	      {
		std::reverse(round.begin(), round.end());
		result.push_back(round);
	      }
	    curr = curr->prevRound;
	    round = std::vector<std::pair<int, const CribbageCard *>>();
	  }
	else
	  {
	    curr = curr->prevPlay;
	  }
      }
    std::reverse(result.begin(), result.end());
    return result;
  }

}

