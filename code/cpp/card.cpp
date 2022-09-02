namespace cpsc474
{

  template<class R, class S>
  Card<R, S>::Card(const R& r, const S& s) : rank(r), suit(s)
  {
    hashValue = std::hash<R>()(r) ^ std::hash<S>()(s);
  }
  
  /**
   * Returns the rank of this card.
   *
   * @return the rank of this card
   */
  template<class R, class S>
  const R& Card<R, S>::getRank() const
  {
    return rank;
  }
  
  /**
   * Returns the suit of this card.
   *
   * @return the suit of this card
   */
  template<class R, class S>
  const S& Card<R, S>::getSuit() const
  {
    return suit;
  }

  template<class R, class S>
  bool Card<R, S>::operator==(const Card<R, S>& other) const
  {
    return suit == other.suit && rank == other.rank;
  }
  
  template<class R, class S>
  void Card<R, S>::print(std::ostream& os) const
  {
    os << rank << suit;
  }

  /**
   * Write this card to the given stream.
   *
   * @param c a card
   * @param os an output stream
   * @return that stream
   */
  template<class R, class S>
  std::ostream& operator<<(std::ostream& os, const Card<R, S>& c)
  {
    c.print(os);
    return os;
  }
}
