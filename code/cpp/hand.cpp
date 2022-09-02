#include <vector>
#include <unordered_map>

#include "hand.h"
#include "utility.h"

namespace cpsc474
{
  CribbageHand::CribbageHand(const std::vector<const CribbageCard *>& cards)
  {
    std::copy(cards.begin(), cards.end(), std::back_inserter(hand));
  }

  CribbageHand::CribbageHand(const CribbageHand& h1, const CribbageHand& h2)
  {
    std::copy(h1.hand.begin(), h1.hand.end(), std::back_inserter(hand));
    std::copy(h2.hand.begin(), h2.hand.end(), std::back_inserter(hand));
  }

  size_t CribbageHand::size() const
  {
    return hand.size();
  }

  std::vector<CribbageHand*> CribbageHand::split(const std::vector<size_t>& splitI) const
  {
    // make list of cards for the two parts of the split
    std::vector<std::vector<const CribbageCard *>> temp;
    temp.push_back(std::vector<const CribbageCard *>());
    temp.push_back(std::vector<const CribbageCard *>());

    size_t curr = 0;
    for (const auto& i : splitI)
      {
	// all cards up to the one to split off go into the first part of the split
	while (curr < i)
	  {
	    temp[0].push_back(hand[curr]);
	    curr++;
	  }

	// card to split goes into the second part
	temp[1].push_back(hand[i]);
	curr = i + 1;
      }

    // cards after the last one to split go into the first part
    while (curr < hand.size())
      {
	temp[0].push_back(hand[curr]);
	curr++;
      }

    // turn lists of cards into hands
    std::vector<CribbageHand *> result;
    for (size_t i = 0; i < 2; i++)
      {
	result.push_back(new CribbageHand(temp[i]));
      }
    return result;    
  }
  
  const std::vector<const CribbageCard *>& CribbageHand::cards() const
  {
    return hand;
  }
  
  CribbageHand CribbageHand::remove(const CribbageCard *card) const
  {
    std::vector<const CribbageCard *> retained;
    for (const auto& i : hand)
      {
	if (card != nullptr && *card == *i)
	  {
	    // skip this occurrence but copy any future occurrences
	    card = nullptr;
	  }
	else
	  {
	    retained.push_back(i);
	  }
      }
    return CribbageHand(retained);
  }

  bool CribbageHand::isLegalSplit(const std::vector<const CribbageHand *>& split) const
  {
    // count cards in this hand
    std::unordered_map<const CribbageCard *, size_t, PointerHash<const CribbageCard *, const CribbageCard> , PointerDeepEquals<const CribbageCard *, const CribbageCard>> cardCount;
    for (const CribbageCard *c : hand)
      {
	if (cardCount.count(c) == 0)
	  {
	    cardCount[c] = 1;
	  }
	else
	  {
	    cardCount[c]++;
	  }
      }

    // match cards in split with cards in hand
    size_t partitionSize = 0;
    for (const CribbageHand *part : split)
      {
	partitionSize += part->size();
	for (const CribbageCard *c : part->cards())
	  {
	    if (cardCount.count(c) == 0 || cardCount[c] == 0)
	      {
		// card is not in hand
		return false;
	      }
	  }
      }

    // if we made it this far with no mismatches then parition is legal
    return partitionSize == size();
  }

  
  std::ostream& operator<<(std::ostream& os, const CribbageHand& hand)
  {
    const std::vector<const CribbageCard*>& cards = hand.cards();
    for (const auto& i : cards)
      {
	os << *i << " ";
      }
    return os;
  }
}
