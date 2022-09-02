#include "game.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iterator>

#include "history.h"
#include "card.h"
#include "rank.h"
#include "ordered_ranks.h"
#include "character_suits.h"

namespace cpsc474
{
  std::string RANK_NAMES[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "T|10", "J", "Q", "K"};
  
  std::string CribbageGame::DEFAULT_SUITS = "SHDC";
  std::vector<std::string> CribbageGame::DEFAULT_RANKS = std::vector<std::string>(RANK_NAMES, RANK_NAMES + 13);
 
  CribbageGame::CribbageGame() : pairValue(2), keepCards(4), throwCards(2), scoringSum(15), sumValue(2), peggingLimit(31), winningScore(121), heelsValue(2), cardRanks(DEFAULT_RANKS), cardSuits(std::string("SHDC")), full(cardRanks, cardSuits, 1)
  {
    dealCards = keepCards + throwCards;
    for (int i = 1; i <= 10; i++)
      {
	allValues.push_back(i);
      }
    minValue = 1;
    maxValue = 10;

    numRanks = cardRanks.allRanks().size();
    
    std::vector<size_t> indices;
    for (size_t i = 0; i < dealCards; i++)
      {
	indices.push_back(i);
      }
    Combinations<size_t> combos = Combinations<size_t>(indices, throwCards);
    for (const auto& combination : combos)
      {
	allThrows.push_back(combination);
      }


    for (size_t j = 0; j <= keepCards + 1; j++)
      {
	// make a vector with all possible indices into hands of size j
	std::vector<size_t> handIndices;
	{
	  for (size_t i = 0; i < j; i++)
	    {
	      handIndices.push_back(i);
	    }
	}

	handSubsets.push_back(std::vector<std::vector<std::vector<size_t>>>());
	// run over all sizes of subsets of those indices
	for (size_t sz = 0; sz <= j; sz++)
	  {
	    handSubsets[j].push_back(std::vector<std::vector<size_t>>());
	    for (const auto& subset : Combinations<size_t>(handIndices, sz))
	      {
		std::vector<size_t> copy;
		std::copy(subset.begin(), subset.end(), std::back_inserter(copy));
		handSubsets[j][sz].push_back(copy);
	      }
	  }
      }
  }

  std::vector<int> CribbageGame::score(const CribbageHand& hand, const CribbageCard *turn, bool isCrib) const
  {
    // make one hand with turn card if given
    CribbageHand allCards = turn == nullptr ? hand : CribbageHand(hand, CribbageHand(std::vector<const CribbageCard *>(1, turn)));
    
    int rankCount[numRanks];
    for (const auto& r : cardRanks.getRanks())
      {
	rankCount[r->intValue()] = 0;
      }
    
    for (const auto& c : allCards.cards())
      {
	rankCount[c->getRank().intValue()]++;
      }
    
    int fifteens = 0;
    for (size_t count = 2; count <= allCards.size(); count++)
      {
	for (const auto& indices : handSubsets[allCards.size()][count])
	  {
	    int sum = 0;
	    for (const auto& i : indices)
	      {
		sum += rankValue(allCards.cards()[i]->getRank());
	      }
	    fifteens += sumScore(sum);
	  }
      }
  
    int pairs = 0;
    for (const auto& r : cardRanks.getRanks())
      {
	int count = rankCount[r->intValue()];
	pairs += count * (count - 1) / 2; // number of ways to select 2 cards from count
      }
    pairs *= pairValue;
    
    int runs = 0;
    int currRun = 0;
    int combinations = 1;
    for (const auto& r : cardRanks.getRanks())
      {
	if (rankCount[r->intValue()] == 0)
	  {
	    runs += runValue(currRun, combinations);
	    currRun = 0;
	    combinations = 1;
	  }
	else
	  {
	    currRun += 1;
	    combinations *= rankCount[r->intValue()];
	  }
      }
    runs += runValue(currRun, combinations);
    
    int flushes = 0;
    size_t suitMatches = 0;
    char handSuit = hand.cards()[0]->getSuit();
    for (const auto& c : hand.cards())
      {
	if (c->getSuit() == handSuit)
	  {
	    suitMatches++;
	  }
      }
    if (suitMatches == hand.size() && turn != nullptr && turn->getSuit() == handSuit)
      {
	flushes += turnFlushValue(suitMatches + 1);
      }
    else if (suitMatches == hand.size() && !isCrib)
      {
	flushes += handFlushValue(hand.size());
      }
    
    int nobs = 0;
    for (const auto& c : hand.cards())
      {
	if (turn != nullptr && c->getSuit() == turn->getSuit())
	  {
	    nobs += nobValue(c->getRank());
	  }
      }
    
    return std::vector<int> {pairs + fifteens + runs + flushes + nobs, pairs, fifteens, runs, flushes, nobs};
  }

  std::vector<CribbageHand *> CribbageGame::deal() const
  {
    Deck<CardRank, char, OrderedRanks, CharacterSuits> deck = full.createDeck();
    deck.shuffle();
    
    std::vector<const CribbageCard *> bothHandsAndTurn = deck.deal(dealCards * 2 + 1);
    CribbageHand *p0Hand = new CribbageHand(std::vector<const CribbageCard *>(bothHandsAndTurn.begin(), bothHandsAndTurn.begin() + dealCards));
    CribbageHand *p1Hand = new CribbageHand(std::vector<const CribbageCard *>(bothHandsAndTurn.begin() + dealCards, bothHandsAndTurn.begin() + dealCards * 2));
    CribbageHand *turnCard = new CribbageHand(std::vector<const CribbageCard *>(bothHandsAndTurn.begin() + dealCards * 2, bothHandsAndTurn.begin() + dealCards * 2 + 1));
    
    return std::vector<CribbageHand *> {p0Hand, p1Hand, turnCard};
  }

  std::vector<int> CribbageGame::play(CribbagePolicy *p0, CribbagePolicy *p1, std::ostream& logger) const
  {
    std::vector<int> scores {0, 0};
    std::vector<CribbagePolicy *> policies {p0, p1};
    int dealer = 0;
    int handsPlayed = 0;

    while (*std::max_element(scores.begin(), scores.end()) < winningScore)
      {
	// std::cerr << "Dealing at " + Arrays.toString(scores));
	// deal cards
	std::vector<CribbageHand *> cardsInPlay = deal();
	handsPlayed++;
	// turned card is first (only) element of third part of deal
	const CribbageCard *turn = cardsInPlay[2]->cards()[0];
	logger << "TURN: " << *turn << std::endl;
		
	// check for 2 for heels (turned card is a Jack)
	int heels = turnCardValue(turn);
	scores[dealer] += heels;
	if (heels != 0)
	  {
	    logger << "[";
	    std::copy(scores.begin(), scores.end(), std::ostream_iterator<int>(logger, " "));
	    logger << "]" << std::endl;
	  }
	
	// get keep/throw for each player
	std::vector<std::vector<CribbageHand *>> keeps;
	for (int p = 0; p < 2; p++)
	  {
	    keeps.push_back(policies[p]->keep(*cardsInPlay[p], scores, p == dealer));
	    std::vector<const CribbageHand *> const_keeps;
	    std::copy(keeps[p].begin(), keeps[p].end(), std::back_inserter(const_keeps));
	    if (!cardsInPlay[p]->isLegalSplit(const_keeps))
	      {
		throw std::string("invalid partition");
	      }
	    if (keeps[p][0]->size() != keepCards)
	      {
		throw std::string("invalid size for partition");
	      }
	  }
	
	// initialize pegging
	int pegTurn = 1 - dealer;
	std::vector<PeggingHistory *> historyList;
	PeggingHistory *history = new PeggingHistory(*this);
	historyList.push_back(history);
	std::vector<CribbageHand *> pegCards = {new CribbageHand(*keeps[0][0]),
						new CribbageHand(*keeps[1][0])};
	while (*std::max_element(scores.begin(), scores.end()) < winningScore
	       && !history->isTerminal())
	  {
	    // get player's played card
	    std::vector<int> scoresReversed(scores.begin(), scores.end());
	    std::reverse(scoresReversed.begin(), scoresReversed.end());
	    const CribbageCard *play = policies[pegTurn]->peg(*pegCards[pegTurn], *history, pegTurn == 0 ? scores : scoresReversed, pegTurn == dealer);

	    if (play == nullptr && history->hasLegalPlay(*pegCards[pegTurn], pegTurn == dealer ? 0 : 1))
	      {
		throw std::string("passing with legal play");
	      }
	    if (play != nullptr && !history->isLegal(*play, pegTurn == dealer ? 0 : 1))
	      {
		throw std::string("invalid card played");
	      }
	    
	    history = history->play(play, pegTurn == dealer ? 0 : 1);
	    historyList.push_back(history);

	    if (play != nullptr)
	      {
		logger << pegTurn << " " << *play << std::endl;
	      }
	    else
	      {
		logger << pegTurn << " Pass" << std::endl;
	      }
	    
	    // score the play
	    const std::vector<int>& playScore = history->getScore();
	    if (playScore[0] > 0)
	      {
		scores[pegTurn] += playScore[0];
	      }
	    else if (playScore[0] < 0)
	      {
		scores[1 - pegTurn] += -playScore[0];
	      }
	    
	    if (playScore[0] != 0)
	      {
		// display score if it changed
		logger << "[";
		std::copy(scores.begin(), scores.end(), std::ostream_iterator<int>(logger, " "));
		logger << "]" << std::endl;
	      }
	    
	    // remove played card from hand
	    if (play != nullptr)
	      {
		CribbageHand *retained = new CribbageHand(pegCards[pegTurn]->remove(play));
		if (retained->size() == pegCards[pegTurn]->size())
		  {
		    throw std::string("played card is not in hand");
		  }
		delete pegCards[pegTurn];
		pegCards[pegTurn] = retained;
	      }
	    
	    // next player's turn
	    pegTurn = 1 - pegTurn;
	  }

	// delete histories
	for (PeggingHistory *h : historyList)
	  {
	    delete h;
	  }

	// delete (now empty) cards in hand
	for (CribbageHand *h : pegCards)
	  {
	    delete h;
	  }
	
	// score non-dealer's hand
	if (*std::max_element(scores.begin(), scores.end()) < winningScore)
	  {
	    std::vector<int> handScore = score(*keeps[1 - dealer][0], turn, false);
	    scores[1 - dealer] += handScore[0];
	    
 	    logger << "NON-DEALER: " << *(keeps[1 - dealer][0]) << " [";
	    std::copy(scores.begin(), scores.end(), std::ostream_iterator<int>(logger, " "));
	    logger << "]" << std::endl;
	  }
	
	// score dealer's hand
	if (*std::max_element(scores.begin(), scores.end()) < winningScore)
	  {
	    std::vector<int> handScore = score(*keeps[dealer][0], turn, false);
	    scores[dealer] += handScore[0];

 	    logger << "DEALER: " << *(keeps[dealer][0]) << " [";
	    std::copy(scores.begin(), scores.end(), std::ostream_iterator<int>(logger, " "));
	    logger << "]" << std::endl;
	  }
	
	// score crib
	if (*std::max_element(scores.begin(), scores.end()) < winningScore)
	  {
	    CribbageHand crib = CribbageHand(*keeps[0][1], *keeps[1][1]);
	    std::vector<int> handScore = score(crib, turn, true);
	    scores[dealer] += handScore[0];

 	    logger << "CRIB: " << crib << " [";
	    std::copy(scores.begin(), scores.end(), std::ostream_iterator<int>(logger, " "));
	    logger << "]" << std::endl;
	  }
	
	// change dealer
	dealer = 1 - dealer;

	// delete original deals
	for (CribbageHand *h : cardsInPlay)
	  {
	    delete h;
	  }

	// delete hands
	for (std::vector<CribbageHand *> a : keeps)
	  {
	    for (CribbageHand *h : a)
	      {
		delete h;
	      }
	  }
      }
    logger << "[";
    std::copy(scores.begin(), scores.end(), std::ostream_iterator<int>(logger, " "));
    logger << "]" << std::endl;
    
    return std::vector<int> {gameValue(scores), handsPlayed};
  }

  EvaluationResults CribbageGame::evaluatePolicies(CribbagePolicy *p0, CribbagePolicy *p1, size_t count) const
  {
    std::ostream nullLogger(nullptr);
    size_t totalHands = 0;
    int netPoints = 0;
    int p0Points = 0;
    int p1Points = 0;
    std::unordered_map<int, size_t> overallResults;
    
    for (size_t g = 0; g < count; g++)
      {
	int points;
	std::vector<int> gameResult;
	if (g % 2 == 0)
	  {
	    gameResult = play(p0, p1, nullLogger);
	    points = gameResult[0];
	  }
	else
	  {
	    gameResult = play(p1, p0, nullLogger);
	    points = -gameResult[0];
	  }

	if (overallResults.count(points) == 0)
	  {
	    overallResults[points] = 0;
	  }
	overallResults[points]++;
	totalHands += gameResult[1];
	netPoints += points;
	if (points > 0)
	  {
	    p0Points += points;
	  }
	else
	  {
	    p1Points += -points;
	  }
      }

    return std::make_tuple(std::make_pair((double)p0Points / count, (double)p1Points/ count), overallResults, (double)totalHands / count);
  }

  
  int CribbageGame::gameValue(const std::vector<int>& scores) const
  {
    if (*std::max_element(scores.begin(), scores.end()) < winningScore)
      {
	return 0;
      }
    
    int points = 1;
    int loserScore = *std::min_element(scores.begin(), scores.end());
    if (loserScore <= 60)
      {
	points = 3;
      }
    else if (loserScore <= 90)
      {
	points = 2;
      }
    
    return (scores[0] > scores[1] ? 1 : -1) * points;
  }
  
}
