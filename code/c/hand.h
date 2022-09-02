#ifndef __HAND_H__
#define __HAND_H__

#include <ostream>
#include <vector>

#include "rank.h"
#include "card.h"

namespace cpsc474
{
  typedef Card<CardRank, char> CribbageCard;
  
  class CribbageHand
  {
  public:
    /**
     * Creates a hand from the given cards.
     * Ownership of the cards is not transferred and their lifetime is not
     * affected and must be longer than the lifetime of the new hand in order
     * for the new hand to remain valid.
     *
     * @param c an vector of non-null pointers to CribbageCards
     */
    CribbageHand(const std::vector<const CribbageCard *>& cards);

    /**
     * Creates a new hand containing the cards from one hand in
     * addition to the cards from the other hand.  If there are
     * duplicates of a card, then the new hand will contain the
     * corresponding number of copies of that card.  Ownership and
     * lifetime of the cards within the new hand is the same as that
     * of the cards in the original hands.
     *
     * @param h1 a CribbageHand
     * @param h2 a CribbageHand
     */
    CribbageHand(const CribbageHand&, const CribbageHand& h2);

    /**
     * Returns the size of this hand.
     *
     * @return the size of this hand
     */
    size_t size() const;

    /**
     * Creates the two hands that result from removing the cards at
     * the given indices in this hand.  Pointers to the new hands are
     * returned in a vector and the caller takes ownership of the new
     * hands, which must eventually be destroyed with delete.
     * Ownership and lifetime of the cards within those hands is the
     * same as that of the cards in the original hand.
     *
     * @param splitI an array of strictly increasing indices into this hand
     * @param split an array to overwrite with pointers to the new hands
     */
    std::vector<CribbageHand *> split(const std::vector<size_t>& splitI) const;

    /**
     * Returns a vector containing pointer to the cards in this hand.
     * Ownership and lifetime of those cards is the same as those in
     * this gane.
     *
     * @return a vector of pointers to cards in this hand
     */
    const std::vector<const CribbageCard *>& cards() const;

    CribbageHand remove(const CribbageCard* card) const;

    bool isLegalSplit(const std::vector<const CribbageHand *>& split) const;
    
  private:
    std::vector<const CribbageCard *> hand;
  };

  std::ostream& operator<<(std::ostream& os, const CribbageHand& hand);
  
}

#endif

