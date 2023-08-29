#include "c_greedy.h"

#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "c_cribbage.h"
#include "c_utility.h"

void greedy_keep(const void *game, const void *hand, int scores[2], int am_dealer, void *split[2])
{
  for (size_t i = 0; i < 2; i++)
    {
      split[i] = NULL;
    }
  int best_net = INT_MIN;
  size_t ties = 0;
  for (size_t throw1 = 0; throw1 < 6; throw1++)
    {
      for (size_t throw2 = throw1 + 1; throw2 < 6; throw2++)
	{
	  void *curr_split[2];
	  size_t indices[2] = { throw1, throw2 };
	  hand_split(hand, indices, curr_split);
	  
	  int net_points = (game_score(game, curr_split[0], NULL, false)
			    + (am_dealer ? 1 : -1) * game_score(game, curr_split[1], NULL, true));
	  if (net_points > best_net)
	    {
	      for (size_t i = 0; i < 2; i++)
		{
		  hand_destroy(split[i]);
		}
	      memcpy(split, curr_split, sizeof(void *) * 2);
	      best_net = net_points;
	      ties = 0;
	    }
	  else if (net_points == best_net && ties++ >= 0 && random01() < 1.0 / ties)
	    {
	      for (size_t i = 0; i < 2; i++)
		{
		  hand_destroy(split[i]);
		}
	      memcpy(split, curr_split, sizeof(void *) * 2);
	    }
	  else
	    {
	      for (size_t i = 0; i < 2; i++)
		{
		  hand_destroy(curr_split[i]);
		}
	    }
	}
    }
}

const void *greedy_peg(const void *game, const void *hand, const void *hist, const void *turn, int scores[2], int am_dealer)
{
  const void *selected = NULL;
  size_t ties = 0;
  int best_score = INT_MIN;
  size_t size = hand_size(hand);
  for (size_t i = 0; i < size; i++)
    {
      const void *c = hand_card(hand, i);
      int card_score = history_score(hist, c, am_dealer ? 0 : 1);
      if (card_score > INT_MIN)
	{
	  // card is a valid play
	  if (card_score > best_score)
	    {
	      best_score = card_score;
	      selected = c;
	      ties = 0;
	    }
	  else if (card_score == best_score)
	    {
	      ties++;
	      
	      if (random01() < 1.0 / ties)
		{
		  // effect is to uniformly randomly select from tied cards
		  selected = c;
		}
	    }
	  
	}
    }
  return selected;
}

