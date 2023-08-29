#include "c_cribbage.h"

#include <iterator>
#include <vector>
#include <iostream>

#include "cstdlib"

#include "rank.h"
#include "ordered_ranks.h"
#include "character_suits.h"
#include "deck.h"
#include "hand.h"
#include "history.h"
#include "game.h"

using cpsc474::CribbageCard;
using cpsc474::CribbageHand;
using cpsc474::PeggingHistory;
using cpsc474::CribbageGame;
using cpsc474::Deck;
using cpsc474::OrderedRanks;
using cpsc474::CardRank;
using cpsc474::CharacterSuits;

extern "C"
{
  int game_score(const void *g, const void *h, const void *t, int isCrib)
  {
    const CribbageGame *game = (const CribbageGame *)g;
    const CribbageHand *hand = (const CribbageHand *)h;
    const CribbageCard *turn = (const CribbageCard *)t;

    std::vector<int> scores = game->score(*hand, turn, isCrib);
    return scores[0];
  }

  int game_subscores(const void *g, const void *h, const void *t, int isCrib, int subscores[5])
  {
    const CribbageGame *game = (const CribbageGame *)g;
    const CribbageHand *hand = (const CribbageHand *)h;
    const CribbageCard *turn = (const CribbageCard *)t;

    std::vector<int> scores = game->score(*hand, turn, isCrib);
    if (subscores != NULL)
      {
	for (size_t i = 0; i < 5; i++)
	  {
	    subscores[i] = scores[i + 1];
	  }
      };
    
    return scores[0];
  }

  void *game_deck(const void *g)
  {
    const CribbageGame *game = (const CribbageGame *)g;

    return new Deck<CardRank, char, OrderedRanks, CharacterSuits>(game->fullDeck());
  }

  int game_rank_value(const void *g, int rank)
  {
    if (rank < 9)
      {
	// 0 = 1, 1 = 2, ..., 8 = 9
	return rank + 1;
      }
    else
      {
	// 9 = 10; 10 = J, 11 = Q, 12 = K
	return 10;
      }
  }

  void deck_shuffle(void *d)
  {
    ((Deck<CardRank, char, OrderedRanks, CharacterSuits> *)d)->shuffle();
  }
  
  size_t deck_size(const void *d)
  {
    return ((Deck<CardRank, char, OrderedRanks, CharacterSuits> *)d)->size();
  }
  
  void deck_remove(void *d, const void *cards[], size_t n)
  {
    Deck<CardRank, char, OrderedRanks, CharacterSuits> *deck = (Deck<CardRank, char, OrderedRanks, CharacterSuits> *)d;

    std::vector<const CribbageCard *> c;
    for (size_t i = 0; i < n; i++)
      {
	c.push_back((const CribbageCard *)cards[i]);
      }

    deck->remove(c);
  }

  const void **deck_deal(void *d, size_t n)
  {
    Deck<CardRank, char, OrderedRanks, CharacterSuits> *deck = (Deck<CardRank, char, OrderedRanks, CharacterSuits> *)d;

    std::vector<const CribbageCard *> cards = deck->deal(n);
    
    const CribbageCard **result = (const CribbageCard **)malloc(sizeof(*result) * n);
    for (size_t i = 0; i < cards.size(); i++)
      {
	result[i] = cards[i];
      }
    return (const void **)result;
  }

  const void **deck_peek(const void *d, size_t n)
  {
    const Deck<CardRank, char, OrderedRanks, CharacterSuits> *deck = (const Deck<CardRank, char, OrderedRanks, CharacterSuits> *)d;
    
    std::vector<const CribbageCard *> cards = deck->peek(n);
    
    const CribbageCard **result = (const CribbageCard **)malloc(sizeof(*result) * n);
    for (size_t i = 0; i < cards.size(); i++)
      {
	result[i] = cards[i];
      }
    return (const void **)result;
  }

  void deck_destroy(void *d)
  {
    delete (Deck<CardRank, char, OrderedRanks, CharacterSuits> *)d;
  }

  int card_rank(const void *c)
  {
    return ((const CribbageCard *)c)->getRank().intValue();
  }

  char card_suit(const void *c)
  {
    return ((const CribbageCard *)c)->getSuit();
  }

  void card_print(const void *c)
  {
    std::cout << *((const CribbageCard *)c);
  }

  bool history_is_start_round(const void *h)
  {
    return ((const PeggingHistory *)h)->startRound();
  }

  int history_total(const void *h)
  {
    return ((const PeggingHistory *)h)->getTotal();
  }

  bool history_player_passed(const void *h, int p)
  {
    return ((const PeggingHistory *)h)->hasPassed(p);
  }
  
  int history_score(const void *h, const void *c, int player)
  {
    const PeggingHistory *hist = (const PeggingHistory *)h;
    const CribbageCard *card = (const CribbageCard *)c;
    
    std::vector<int> scores = hist->score(card, player);
    if (scores.size() > 0)
      {
	return scores[0];
      }
    else
      {
	// invalid card
	return INT_MIN;
      }
  }

  int history_subscores(const void *h, const void *c, int player, int subscores[4])
  {
    const PeggingHistory *hist = (const PeggingHistory *)h;
    const CribbageCard *card = (const CribbageCard *)c;

    std::vector<int> scores = hist->score(card, player);
    if (subscores != NULL && scores.size() > 0)
      {
	for (size_t i = 0; i < 4; i++)
	  {
	    subscores[i] = scores[i + 1];
	  }
      }
    else if (scores.size() == 0)
      {
	return INT_MIN;
      }
    
    return scores[0];
  }

  pegging_play **history_plays(const void *h, size_t *rows, size_t **cols)
  {
    const PeggingHistory *history = (const PeggingHistory *)h;
    auto plays = history->plays();
    *rows = plays.size();
    *cols = (size_t *)malloc(sizeof(size_t) * *rows);
    pegging_play **result = (pegging_play **)malloc(sizeof(*result) * (*rows));
    if ((*cols) == NULL || result == NULL)
      {
	free(*cols);
	free(result);
	*rows = 0;
	*cols = NULL;
	return NULL;
      }
    for (size_t r = 0; r < *rows; r++)
      {
	(*cols)[r] = plays[r].size();
	result[r] = (pegging_play *)malloc(sizeof(pegging_play) * (*cols)[r]);
	for (size_t c = 0; c < (*cols)[r]; c++)
	  {
	    result[r][c].player = plays[r][c].first;
	    result[r][c].card = plays[r][c].second;
	  }
      }

    return result;
  }
    

  void *hand_create(const void *c[], size_t n)
  {
    std::vector<const CribbageCard *> cards;
    for (size_t i = 0; i < n; i++)
      {
	cards.push_back((const CribbageCard *)c[i]);
      }
    return new CribbageHand(cards);
  }
  
  void *hand_merge(const void *h1, const void *h2)
  {
    return new CribbageHand(*((const CribbageHand *)h1), *((const CribbageHand *)h2)); 
  }
  
  const void *hand_card(const void *h, int index)
  {
    return ((const CribbageHand *)h)->cards()[index];
  }

  const void **hand_cards(const void *h)
  {
    const CribbageHand *hand = (const CribbageHand *)h;
    size_t size = hand->size();
    const void **cards = (const void **)malloc(sizeof(const CribbageCard *) * size);
    if (cards != NULL)
      {
	for (size_t i = 0; i < size; i++)
	  {
	    cards[i] = hand->cards()[i];
	  }
      }
    return cards;
  } 

  size_t hand_size(const void *h)
  {
    return ((const CribbageHand *)h)->size();
  }

  void hand_split(const void *h, size_t discard[2], void *split[2])
  {
    const CribbageHand *hand = (const CribbageHand *)h;
    std::vector<size_t> indices(discard, discard + 2);
    std::vector<CribbageHand *> subhands = hand->split(indices);
    for (size_t i = 0; i < 2; i++)
      {
	split[i] = subhands[i];
      }
  }

  void hand_print(const void *h)
  {
    std::cout << *((const CribbageHand *)h);
  }
  
  void hand_destroy(void *h)
  {
    delete (CribbageHand *)h;
  }

}

  
