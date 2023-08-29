#ifndef __POLICY_H__
#define __POLICY_H__

#include <vector>

#include "hand.h"
#include "history.h"

namespace cpsc474
{
  class CribbageGame;
  
  class KeepPolicy
  {
  public:
    virtual ~KeepPolicy() {}
    
    /**
     * Selects two cards to contribute to the crib from the given hand.
     * The choice is indicated by two CribbageHands that the function
     * returns pointers to in returned vector.  Ownership of those two
     * CribbageHands is transferred to the caller and they must
     * eventually be destroyed with hand_destroy.  Ownership and
     * lifetime of the cards contained in those hands is the same as the
     * cards in the original hand.
     *
     * @param cards a CribbageHand
     * @param scores an vector of integers both less than the score needed to win
     * @param am_dealer true if this policy is playing as the dealer,
     *        false othwewise
     * @return a vector containing pointers to CribbageHands that partition
     *         the original hand, with two cards in the second element
     */
    virtual std::vector<CribbageHand *> keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer) = 0;
  };

  class PegPolicy
  {
  public:
    virtual ~PegPolicy() {}
    
    /**
     * Selects a card to play during the pegging (play) phase.  The return
     * value is the chosen card, or nullptr if there is no card that is legal
     * to play.  Ownership and lifetime of the returned card is the same
     * as the cards in the hand to play from.
     *
     * @param hand a CribbageHand
     * @param hist a PeggingHistory
     * @param turn a CribbageCard
     * @param scores an vector of integers both less than the score needed
     *        to win the game
     * @param am_dealer true if the policy is playing as the dealer,
     *        false otherwise
     * @return a pointer to a CribbageCard contained in hand that is legal
     *         to play given the given history, or nullptr if no such card
     */
    virtual const CribbageCard *peg(const CribbageHand& cards, const PeggingHistory& hist, const CribbageCard& turn, const std::vector<int>& scores, bool amDealer) = 0;
  };
  
  class CribbagePolicy : public KeepPolicy, public PegPolicy
  {
  public:
    virtual ~CribbagePolicy() {}
  };

  class CompoundPolicy : public CribbagePolicy
  {
  public:
    CompoundPolicy(KeepPolicy& keepPolicy, PegPolicy& pegPolicy)
      : keepPolicy(keepPolicy), pegPolicy(pegPolicy) {}

    std::vector<CribbageHand *> keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer) { return keepPolicy.keep(cards, scores, amDealer); }

    const CribbageCard *peg(const CribbageHand& cards, const PeggingHistory& hist, const CribbageCard& turn, const std::vector<int>& scores, bool amDealer) { return pegPolicy.peg(cards, hist, turn, scores, amDealer); }
    
  protected:
    KeepPolicy& keepPolicy;
    PegPolicy& pegPolicy;
  };

  class RandomThrower : public KeepPolicy
  {
  public:
  RandomThrower(const CribbageGame& game) : game(game) {}

    std::vector<CribbageHand *> keep(const CribbageHand& cards, const std::vector<int>& scores, bool amDealer);

  protected:
    const CribbageGame& game;
  };

  class RandomPegger : public PegPolicy
  {
  public:
  RandomPegger(const CribbageGame& game) : game(game) {}

    const CribbageCard *peg(const CribbageHand& cards, const PeggingHistory& hist, const CribbageCard& turn, const std::vector<int>& scores, bool amDealer);

  protected:
    const CribbageGame& game;
  };
}

#endif
