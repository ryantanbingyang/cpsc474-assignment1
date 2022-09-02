#ifndef __CARD_H__
#define __CARD_H__

#include <ostream>

namespace cpsc474
{
  template<class R, class S>
    class Card
    {
    public:
      Card(const R& r, const S& s);
      
      /**
       * Returns the rank of this card.
       *
       * @return the rank of this card
       */
      const R& getRank() const;
      
      /**
       * Returns the suit of this card.
       *
       * @return the suit of this card
       */
      const S& getSuit() const;
      
      bool operator==(const Card<R, S>& o) const;

      bool operator!=(const Card<R, S>& o) const { return !(*this == o); }
      
      void print(std::ostream& os) const;

      size_t hash() const { return hashValue; }

    private:
      const R& rank;
      const S& suit;

      size_t hashValue;
    };

  template<class R, class S>
    std::ostream& operator<<(std::ostream& os, const Card<R, S>& card);
  
}

namespace std
{
  template<class R, class S>
    class hash<const cpsc474::Card<R, S>>
    {
    public:
      size_t operator()(const cpsc474::Card<R, S>& card) const { return card.hash(); }
    };
}

#include "card.cpp"

#endif
