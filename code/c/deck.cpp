#include <cstdlib>

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "rank.h"
#include "card.h"
#include "utility.h"

namespace cpsc474
{

  template<class R, class S, class RT, class ST>
  Deck<R, S, RT, ST>::Deck(const std::vector<const Card<R, S> *> cards)
    : cards(cards)
  {
  }
  
  template<class R, class S, class RT, class ST>
  size_t Deck<R, S, RT, ST>::size() const
  {
    return cards.size();
  }
  
  template<class R, class S, class RT, class ST>
  void Deck<R, S, RT, ST>::shuffle()
  {
    size_t numCards = size();
    for (size_t c = 0; c < numCards - 1; c++)
      {
	size_t exchangePos = random() % (numCards - c);
	std::swap(cards[exchangePos], cards[numCards - 1 - c]);
      }
  }

  template<class R, class S, class RT, class ST>
  std::vector<const Card<R, S> *> Deck<R, S, RT, ST>::deal(size_t n)
  {
    std::vector<const Card<R, S>*> result;
    
    for (size_t i = 0; i < n; i++)
      {
	result.push_back(cards[cards.size() - 1]);
	cards.pop_back();
      }

    return result;
  }

  template<class R, class S, class RT, class ST>
  std::vector<const Card<R, S> *> Deck<R, S, RT, ST>::peek(size_t n) const
  {
    std::vector<const Card<R, S> *> result;
    
    for (size_t i = 0; i < n; i++)
      {
	result.push_back(cards[cards.size() - 1 - i]);
      }

    return result;
  }

  template<class R, class S, class RT, class ST>
  template<class L>
  void Deck<R, S, RT, ST>::remove(const L& toRemove)
  {
    std::vector<const Card<R, S> *> remaining;
    std::unordered_map<const Card<R, S> *, size_t, PointerHash<const Card<R, S> *, const Card<R, S>> , PointerDeepEquals<const Card<R, S> *, const Card<R, S>>> removeCount;
    for (auto i : toRemove)
      {
	if (removeCount.count(i) == 0)
	  {
	    removeCount[i] = 1;
	  }
	else
	  {
	    removeCount[i]++;
	  }
      }

    for (auto c : cards)
      {
	if (removeCount.count(c) == 0 || removeCount[c] == 0)
	  {
	    remaining.push_back(c);
	  }
	else
	  {
	    removeCount[c]--;
	  }
      }

    cards = remaining;
  }

  template<class R, class S, class RT, class ST>
  DeckType<R, S, RT, ST>::DeckType(const RT& ranks, const ST& suits, size_t n)
    : ranks(ranks), suits(suits)
  {
    const std::vector<const R *> allRanks = ranks.allRanks();
    const std::vector<const S *> allSuits = suits.allSuits();
    size_t numRanks = allRanks.size();
    size_t numSuits = allSuits.size();
    for (size_t r = 0; r < numRanks; r++)
      {
	for (size_t s = 0; s < numSuits; s++)
	  {
	    Card<R, S> *card = new Card<R, S>(*allRanks[r], *allSuits[s]);
	    uniqueCards.push_back(card);
	    for (size_t c = 0; c < n; c++)
	      {
		fullDeck.push_back(card);
	      }
	  }
      }
  }

  template<class R, class S, class RT, class ST>
  DeckType<R, S, RT, ST>::~DeckType()
  {
    for (Card<R, S> *card : uniqueCards)
      {
	delete card;
      }
  }
  
  template<class R, class S, class RT, class ST>
  Deck<R, S, RT, ST> DeckType<R, S, RT, ST>::createDeck() const
  {
    return Deck<R, S, RT, ST>(fullDeck);
  }

  template<class R, class S, class RT, class ST>
  const Card<R, S> *DeckType<R, S, RT, ST>::parseCard(const std::string& card) const
  {
    const CardRank *rank = ranks.parseRank(card.substr(0, card.size() - 1));
    const char *suit = suits.parseSuit(card[card.size() - 1]);

    if (rank == nullptr || suit == nullptr)
      {
	// invlaid rank or suit
	return nullptr;
      }
    
    Card<R, S> temp(*rank, *suit);

    // we want to return the existing card, not the copy; search for a match
    // this is only used for unit tests and interactive play, so sequential
    // search should be fine
    size_t i = 0;
    while (i < uniqueCards.size() && *uniqueCards[i] != temp)
      {
	i++;
      }
    
    if (i < uniqueCards.size())
      {
	// matching card was found
	return uniqueCards[i];
      }
    else
      {
	// matching card not found
	return nullptr;
      }
  }
}
