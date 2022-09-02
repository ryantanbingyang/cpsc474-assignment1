#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <vector>

#include "hand.h"

namespace cpsc474
{
  extern std::vector<int> zeroScore;
  extern std::vector<bool> noPasses;
  extern std::vector<size_t> zeroPlayed;
  
  class CribbageGame;
  
  class PeggingHistory
  {
  public:
    PeggingHistory(const CribbageGame& game, const PeggingHistory *prevPlay = nullptr, const PeggingHistory *prevRound = nullptr, int total = 0, const std::vector<bool>& passed = noPasses, const std::vector<size_t>& played = zeroPlayed, const CribbageCard *card = nullptr, int player = -1, const std::vector<int>& score = zeroScore);
    
    bool isTerminal() const;
    const std::vector<int>& getScore() const;
    
    /**
     * Returns the point total after the last played in this history,
     * or 0 if no cards have been played or if the last play was a pass
     * to end a round of pegging.
     *
     * @return the point total
     */
    int getTotal() const;

    /**
     * Determines if the given pegging history represents the start of
     * a round of pegging (that is, whether the next card played will be
     * the start of a new count to 31.
     *
     * @return true if this is the start of a round of pegging, false otherwise
     */
    bool startRound() const;
    
    /**
     * Determines if the given player has passed since the last time the count
     * reached 31 or was reset.  If the total from the last card was
     * exactly 31, then the "last time the count reached 31" is this
     * history, and neither player has had a chance to pass since
     * then, so the return value will be false for both players.
     *
     * @param p 0 for the dealer, or 1 for the non-dealer
     * @return true if the player has passed, false otherwise
     */
    bool hasPassed(int player) const;
    
    PeggingHistory *play(const CribbageCard *card, int player) const;
    bool isLegal(const CribbageCard& card, int player) const;
    bool hasLegalPlay(const CribbageHand& hand, int player) const;

    /**
     * Returns the score and subscores earned by the given player when
     * making the given play.  The first element of the returned
     * vector is the total score, and the remaining elements are the
     * subscores in the order pairs, 15s, runs, and last.  The scores
     * are negative to indicate that the other player scores points
     * (as for a "go").  If the play is illegal given this history,
     * the the returned vector is empty.
     *
     * @param c a pointer to a CribbageCard, or nullptr to indicate a pass
     *        ("go")
     * @param p 0 for the dealer, or 1 for the non-dealer
     * @return a vector of integers with the non-zero entries all the
     * same sign and with the first element equal to the sum of the others,
     * or an empty vector
     */
    std::vector<int> score(const CribbageCard *card, int player) const;
    
  private:
    const CribbageGame& game;

    const PeggingHistory *prevPlay;
    const PeggingHistory *prevRound;

    const CribbageCard *card;
    int player;
    std::vector<int> lastScore;

    int total;
    std::vector<bool> passed;
    std::vector<size_t> cardsPlayed;
  };
 
}

#endif
