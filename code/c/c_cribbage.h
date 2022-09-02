#ifndef __C_CRIBBAGE_H__
#define __C_CRIBBAGE_H__

#include <stdlib.h>
#include <stdbool.h>

/**
 * The type of function required of a keep/throw policy.  The action
 * is indicated by two CribbageHands that the function writes pointers to
 * in the split argument.  Ownership of those two CribbageHands is
 * transferred to the caller and they must eventually be destroyed with
 * hand_destroy.  Ownership and lifetime of the cards contained in those
 * hands is the same as the cards in the original hand.
 *
 * @param game a pointer to a CribbageGame, non-null
 * @param hand a pointer to a CribbageHand, non-null
 * @param scores an array of integers both less than the score needed to win
 * @param am_dealer 1 if the policy is playing as the dealer, 0 othwewise
 * @param split an array whose contents can be overwritten with pointers
 * to CribbageHands representing the action to take
 */
typedef void (*KEEP_POLICY)(const void *game, const void *hand, int scores[2], int am_dealer, void *split[2]);

/**
 * The type of function required of a pegging policy.
 *
 * @param game a pointer to a CribbageGame, non-null
 * @param hand a pointer to a CribbageHand, non-null
 * @param a pointer to a PeggingHistory, non-null
 * @param scores an array of integers both less than the score needed to win
 * @param am_dealer 1 if the policy is playing as the dealer, 0 othwewise
 * @param return a pointer to a CribbageCard contained in hand that is
 *        legal to play given the given history, or NULL if no such card
*/
typedef const void *(*PEG_POLICY)(const void *game, const void *hand, const void *hist, int scores[2], int am_dealer);

#ifdef __cplusplus
extern "C"
{
#endif
  /**
   * Returns the score that would be earned by the given hand with the
   * given turn (cut) card.
   *
   * @param g a pointer to a CribbageGame, non-null
   * @param h a pointer to a CribbageHand, non-null
   * @param t a pointer to a CribbageCard, or null
   * @param isCrib 1 to score using crib scoring rules, 0 otherwise
   * @return the score earned
   */
  int game_score(const void *g, const void *h, const void *t, int isCrib);

  /**
   * Returns the subscores that would be earned by the given hand with the
   * given turn (cut) card.  The subscores are written to the array 
   * argument in the order pairs, 15s, runs, flushes, nobs,
   * and the total score is returned.
   *
   * @param g a pointer to a CribbageGame, non-null
   * @param h a pointer to a CribbageHand, non-null
   * @param t a pointer to a CribbageCard, or null
   * @param isCrib 1 to score using crib scoring rules, 0 otherwise
   * @param subscores an array that can be overwritten with the subscores
   * @return the sum of the values written in subscores
   */
  int game_subscores(const void *g, const void *h, const void *t, int isCrib, int subscores[5]);

  /**
   * Returns a complete deck used by the given game.  The caller owns
   * the resulting deck, which must be destroyed using deck_destroy.
   *
   * @param g a pointer to a CribbageGame, non-null
   */
  void *game_deck(const void *g);

  /**
   * Returns the pegging point value of the given rank in the given game.
   *
   * @param g a pointer to a CribbageGame, non-null
   * @param rank a rank that is valid for that game
   * @return the pegging point value of rank
   */
  int game_rank_value(const void *g, int rank);

  /**
   * Shuffles the given deck.
   *
   * @param d a pointer to a Deck, non-NULL
   */
  void deck_shuffle(void *d);

  /**
   * Returns the number of cards remaining in the given deck.
   *
   * @param d a pointer to a Deck, non-NULL
   * @return the number of cardsremaining in that deck
   */
  size_t deck_size(const void *d);

  /**
   * Removes the given cards from the given deck.  Exactly one occurrence of
   * each card is removed for each of its appearances in the array of cards
   * to remove, so to remove two copies of a card, that card should appear
   * twice in the array.  There is no effect for a given card in the array
   * if it is not present in the deck.  The order of the cards after removal
   * is unspecified.
   *
   * @param d a pointer to a Deck, non-NULL
   * @param cards an array of pointers to CribbageCards
   * @param n the size of the cards array
   */
  void deck_remove(void *d, const void *cards[], size_t n);

  /**
   * Returns the top n cards in the given deck and removes them from
   * the deck.  The order of the remaining cards in the deck is
   * unaffected.  The cards are returned as pointers stored in a
   * dynamically allocated array.  The caller owns the resulting array
   * but the deck retains ownership of the cards pointed to by the
   * elements of that array, and their lifetime is the same as the deck's.
   *
   * @param d a pointer to a Deck containing at least n cards, non-null
   * @param n a nonnegative integer no greater than the size of the deck
   * @return a non-null array of non-null pointers to the cards dealt
   *         from the deck
   */
  const void **deck_deal(void *d, size_t n);

  /**
   * Returns the top n cards in the given deck and without removing them from
   * the deck.  The cards are returned as pointers stored in a
   * dynamically allocated array.  The caller owns the resulting array
   * but the deck retains ownership of the cards pointed to by the
   * elements of that array, and their lifetime is the same as the deck's.
   *
   * @param d a pointer to a Deck containing at least n cards, non-null
   * @param n a nonnegative integer no greater than the size of the deck
   * @return a non-null array of non-null pointers to the cards 
   *         from the deck
   */
  const void **deck_peek(const void *d, size_t n);

  /**
   * Destroys the given deck.  Any cards owned by the deck are also
   * destroyed.
   *
   * @param d a pointer to a deck, non-null
   */
  void deck_destroy(void *d);

  /**
   * Returns the rank of the given card.
   *
   * @param c a pointer to a CribbageCard, non-null
   * @return the rank
   */
  int card_rank(const void *c);

  /**
   * Returns the suit of the given card.
   *
   * @param c a pointer to a CribbageCard, non-null
   * @return the suit
   */
  char card_suit(const void *c);

  /**
   * Prints the given card to standard output.
   *
   * @param c a pointer to a CribbageCard, non-null
   */
  void card_print(const void *c);

  /**
   * Determines if the given pegging history represents the start of
   * a round of pegging (that is, whether the next card played will be
   * the start of a new count to 31.
   *
   * @param h a pointer to a PeggingHistory, non-null
   * @return true if h is the start of a round of pegging, false otherwise
   */
  bool history_is_start_round(const void *h);

  /**
   * Returns the point total after the last played in the given history,
   * or 0 if no cards have been played or if the last play was a pass
   * to end a round of pegging.
   *
   * @param h a pointer to a PeggingHistory, non-null
   * @return the point total
   */
  int history_total(const void *h);

  /**
   * Determines if the given player has passed since the last time the count
   * reached 31 or was reset.  If the total from the last card was
   * exactly 31, then the "last time the count reached 31" is the
   * current history, and neither player has had a chance to pass since
   * then, so the return value will be false for both players.
   *
   * @param h a pointer to a PeggingHistory, non-null
   * @param p 0 for the dealer, or 1 for the non-dealer
   * @return true if the player has passed, false otherwise
   */
  bool history_player_passed(const void *h, int p);

  /**
   * Returns the score earned by the given player when making the given
   * play.  The score is negative to indicate that the other player scores
   * points (as for a "go"), and INT_MIN to indicate that the play is illegal.
   *
   * @param h a pointer to a PeggingHistory, non-null
   * @param c a pointer to a CribbageCard, or NULL to indicate a pass ("go")
   * @param p 0 for the dealer, or 1 for the non-dealer
   * @return the score earned by the given player, or negative to indicate
   * points earned by the other player, or INT_MIN to indicat an illegal play
   */
  int history_score(const void *h, const void *c, int player);

  /**
   * Returns the score and subscores earned by the given player when
   * making the given play.  The scores are negative to indicate that
   * the other player scores points (as for a "go").  If the play
   * is illegal given the given history, the the return value is INT_MIN
   * and the output array is unchanged.
   *
   * @param h a pointer to a PeggingHistory, non-null
   * @param c a pointer to a CribbageCard, or NULL to indicate a pass ("go")
   * @param p 0 for the dealer, or 1 for the non-dealer
   * @param subscores an array to overwrite with the subscores earned,
   * in the order pairs, 15s, runs, and "go"s
   * @return the score earned by the given player, or negative to indicate
   * points earned by the other player, or INT_MIN to indicat an illegal play
   */
  int history_subscores(const void *h, const void *c, int player, int subscores[4]);

  /**
   * Creates a hand from the given cards.  The resulting hand is owned
   * by the caller, and must eventually be destroyed with hand_destroy.
   * Ownership of the cards is not transferred and their lifetime is not
   * affected and must be longer than the lifetime of the hand in order
   * for the hand to remain valid.
   *
   * @param c an array of non-null pointers to CribbageCards
   * @param n the size of that array
   * @return a pointer to the newly created hand
   */
  void *hand_create(const void *c[], size_t n);

  /**
   * Returns the card at the given index into the given hand.  The caller
   * does not take ownership of the returned card, whose lifetime remains
   * what it was when it was added to the hand.
   *
   * @param h a pointer to a CribbageHand, non-null
   * @param index an integer between 0 (inclusive) and the size of the hand
   * (exclusive)
   * @return a non-null pointer to the resulting card
   */
  const void *hand_card(const void *h, int index);

  /**
   * Returns an array of non-null pointers to the cards in the given hand.
   * The caller takes ownership of the resulting array, but does not take
   * ownership of cards pointed to by its elements, whose lifetime remains
   * what the same as when they were added to the hand.
   * 
   * @param h a pointer to a CribbageHand, non-null
   * @return a pointer to an array containing pointers to the cards in h
   */
  const void **hand_cards(const void *h);

  /**
   * Returns the size of the given hand.
   *
   * @param h a pointer to a CribbageHand, non-null
   * @return the size of the given hand
   */
  size_t hand_size(const void *h);

  /**
   * Creates the two hands that result from removing the cards at the
   * given indices in the given hand.  Pointers to the new hands are
   * written to the pointer array argument and the caller takes
   * ownership of the new hands, which must eventually be destroyed
   * with hand_destroy.  Ownership and lifetime of the cards within
   * those hands is the same as that of the cards in the original
   * hand.
   *
   * @param h a pointer to a CribbageHand, non-null
   * @param discard an array of strictly increasing indices into that hand
   * @param split an array to overwrite with pointers to the new hands
   */
  void hand_split(const void *h, size_t discard[2], void *split[2]);

  /**
   * Creates a new hand containing the cards from one hand in addition
   * to the cards from the other hand.  If there are duplicates of a
   * card, then the new hand will contain the corresponding number of
   * copies of that card.  The return value is a pointer to the new
   * hand and the caller takes ownership of the new hand, which must
   * eventually be destroyed with hand_destroy.  Ownership and
   * lifetime of the cards within the new hand is the same as that of
   * the cards in the original hands.
   *
   * @param h1 a pointer to a CribbageHand, non-null
   * @param h2 a pointer to a CribbageHand, non-null
   * @return a non-null pointer to a new hand
   */
  void *hand_merge(const void *h1, const void *h2);

  /**
   * Prints the cards contained in the given hand to standard output.
   *
   * @param h a pointer to a CribbageHand, non-null
   */
  void hand_print(const void *h);

  /**
   * Destroys the given hand.  This does not affect any cards contained
   * in the hand.
   *
   * @param h a pointer to a CribbageHand, non-null
   */
  void hand_destroy(void *h);
  

  /**
   * Selects two cards to contribute to the crib from the given hand.
   * The choice is indicated by two CribbageHands that the function
   * writes pointers to in the split argument.  Ownership of those two
   * CribbageHands is transferred to the caller and they must
   * eventually be destroyed with hand_destroy.  Ownership and
   * lifetime of the cards contained in those hands is the same as the
   * cards in the original hand.
   *
   * @param game a pointer to a CribbageGame, non-null
   * @param hand a pointer to a CribbageHand, non-null
   * @param scores an array of integers both less than the score needed to win
   * @param am_dealer 1 if the policy is playing as the dealer, 0 othwewise
   * @param split an array whose contents can be overwritten with pointers
   * to CribbageHands representing the action to take
   */
  void keep(const void *game, const void *hand, int scores[2], int am_dealer, void *split[2]);

  /**
   * Selects a card to play during the pegging (play) phase.  The return
   * value is the chosen card, or NULL if there is no card that is legal
   * to play.  Ownership and lifetime of the returned card is the same
   * as the cards in the hand to play from.
   *
   * @param game a pointer to a CribbageGame, non-null
   * @param hand a pointer to a CribbageHand, non-null
   * @param a pointer to a PeggingHistory, non-null
   * @param scores an array of integers both less than the score needed to win
   * @param am_dealer 1 if the policy is playing as the dealer, 0 othwewise
   * @param return a pointer to a CribbageCard contained in hand that is
   *        legal to play given the given history, or NULL if no such card
   */
  const void *peg(const void *game, const void *hand, const void *hist, int scores[2], int am_dealer);

#ifdef __cplusplus
}
#endif

#endif
