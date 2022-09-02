#ifndef __DECK_H__
#define __DECK_H__

#include <vector>

#include "card.h"

namespace cpsc474
{
  template <class R, class S, class RT, class ST>
    class Deck
  {
  public:
    Deck(const std::vector<const Card<R, S> *> cards);
    
    /**
     * Shuffles this deck.
     */
    void shuffle();

    /**
     * Returns the number of cards remaining in this deck.
     *
     * @return the number of cardsremaining in this deck
     */
    size_t size() const;

    /**
     * Returns the top n cards in this deck and removes them from
     * the deck.  The order of the remaining cards in the deck is
     * unaffected.  The cards are returned as pointers stored in a
     * vector.  The deck retains ownership of the cards pointed to by the
     * elements of that array, and their lifetime is the same as the deck's.
     *
     * @param n a nonnegative integer no greater than the size of this deck
     * @return an vector of non-null pointers to the cards dealt from the deck
     */
    std::vector<const Card<R, S> *> deal(size_t n);

    /**
     * Returns but does not remove the top n cards in this deck.  The
     * cards are returned as pointers stored in a vector.  The deck
     * retains ownership of the cards pointed to by the elements of
     * that array, and their lifetime is the same as the deck's.
     *
     * @param n a nonnegative integer no greater than the size of this deck
     * @return an vector of non-null pointers to the cards from the deck
     */
    std::vector<const Card<R, S> *> peek(size_t n) const;

    /**
     * Removes the given cards from this deck.  Exactly one occurrence of
     * each card is removed for each of its appearances in the cards
     * to remove, so to remove two copies of a card, that card should appear
     * twice in cards.  There is no effect for a given card in cards
     * if it is not present in the deck.  The order of the cards 
     * in this deck after removal is unspecified.
     *
     * @param cards an iterable over pointers to CribbageCards
     */
    template<class L>
      void remove(const L& cards);

  private:
    std::vector<const Card<R, S> *> cards;
    
  };

  template<class R, class S, class RT, class ST>
    class DeckType
  {
  public:
    DeckType(const RT& ranks, const ST& suits, size_t n);

    ~DeckType();

    Deck<R, S, RT, ST> createDeck() const;

    const Card<R, S> *parseCard(const std::string& card) const;

  private:
    std::vector<Card<R, S> *> uniqueCards;
    std::vector<const Card<R, S> *> fullDeck;
    const RT& ranks;
    const ST& suits;
  };
  
}

#include "deck.cpp"

#endif
