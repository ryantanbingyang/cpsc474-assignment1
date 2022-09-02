#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <tuple>
#include <string>

#include "card.h"
#include "hand.h"
#include "deck.h"
#include "ordered_ranks.h"
#include "character_suits.h"
#include "rank.h"
#include "combinations.h"
#include "policy.h"

namespace cpsc474
{
  typedef std::tuple<std::pair<double, double>, std::unordered_map<int, size_t>, double> EvaluationResults;

 class CribbageGame
  {
  public:
    CribbageGame();

    std::vector<int> play(CribbagePolicy *p0, CribbagePolicy *p1, std::ostream& logger = std::cerr) const;

    EvaluationResults evaluatePolicies(CribbagePolicy *p0, CribbagePolicy *p1, size_t count) const;

    /**
     * Returns the score and subscores that would be earned by the given
     * hand with the given turn (cut) card.  The scores are returned
     * in a vector in the order total, pairs, 15s, runs, flushes, nobs.
     *
     * @param h a CribbageHand
     * @param t a pointer to a CribbageCard, or nullptr
     * @param isCrib true to score using crib scoring rules, false otherwise
     * @return a vector of integers where the first element is the total
     * of the remaining elements
     */
    std::vector<int> score(const CribbageHand& hand, const CribbageCard *turn, bool isCrib) const;

    /**
     * Returns a vector containing each combination of indices into a dealt
     * hand that can be chosen as the indices of cards to discard.
     * That is, for the standard game, a vector containing all the
     * size-2 subsets of {0, ..., 5}, where each subset is given as
     * a vector of strictly increasing elements.
     *
     * @return a vector of possible size-2 subsets of indices into a dealt hand
     */
    const std::vector<std::vector<size_t>>& getThrows() const { return allThrows; }

    std::vector<CribbageHand *> deal() const;
      
    /**
     * Returns the pegging point value of the given rank in this game.
     *
     * @param rank a CardRank in this game
     * @return the pegging point value of rank
     */
    int rankValue(const CardRank& r) const { return r.intValue() < 10 ? r.intValue() + 1 : 10; }
    
    size_t cardsToKeep() const { return keepCards; }
    size_t cardsDealt() const { return dealCards; }
    int getPeggingLimit() const { return peggingLimit; }
    int sumScore(int sum) const { return sum == scoringSum ? sumValue : 0; }
    int pegPairValue(int count) const { return 2 * (count * (count - 1) / 2); }
    int pegStraightValue(int length) const { return length >= 3 ? length : 0; }
    int pegSumValue(int sum) const { return sum == 15 ? 2 : 0; }
    int pegExactValue(bool alreadyGo) const { return alreadyGo ? 1 : 2;}
    int handFlushValue(size_t length) const { return length == keepCards ? length : 0; }
    int turnFlushValue(size_t length) const { return length == keepCards + 1 ? length : 0; }
    int runValue(size_t length, size_t combos) const { return length >= 3 ? (length * combos) : 0; }
    int turnCardValue(const CribbageCard *card) const { return card->getRank().getName() == "J" ? heelsValue : 0; }
    int nobValue(const CardRank& rank) const { return rank.getName() == "J" ? 1 : 0; }

    int gameValue(const std::vector<int>& scores) const;

    const CribbageCard *parseCard(const std::string& card) const { return full.parseCard(card); }
    
    /**
     * Returns a complete deck used by this game.
     *
     * @return a complete deck
     */
    Deck<CardRank, char, OrderedRanks, CharacterSuits> fullDeck() const { return full.createDeck(); }
    
  private:
    int pairValue;
    size_t keepCards;
    size_t throwCards;
    int scoringSum;
    int sumValue;
    size_t dealCards;
    int peggingLimit;
    int winningScore;
    int heelsValue;
    const OrderedRanks cardRanks;
    size_t numRanks;
    const CharacterSuits cardSuits;
    std::vector<int> allValues;
    int minValue;
    int maxValue;
    std::vector<std::vector<size_t>> allThrows;
    std::vector<std::vector<std::vector<std::vector<size_t>>>> handSubsets;
    DeckType<CardRank, char, OrderedRanks, CharacterSuits> full;

    static std::string DEFAULT_SUITS;
    static std::vector<std::string> DEFAULT_RANKS;
  };
}

#endif
