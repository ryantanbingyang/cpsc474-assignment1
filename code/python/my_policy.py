from policy import CribbagePolicy, CompositePolicy, GreedyThrower, GreedyPegger
from pegging import Pegging
import scoring
from deck import Deck,Card
from itertools import combinations

class MyPolicy(CribbagePolicy):
    def __init__(self, game):
        self._policy = CompositePolicy(game, GreedyThrower(game), GreedyPegger(game))

        
    def keep(self, hand, scores, am_dealer):
        
        def card_value(card): # function to get the value of a card
            if card.rank > 10:  # face cards are worth 10 points
                return 10
            return card.rank  # otherwise, the value is the rank of the card

        def calculate_combinations(hand): # function to check for combinations (15s, pairs, runs)
            fifteen_combinations = sum(  # calculate fifteens
                1 for pair in combinations(hand, 2) if sum(card_value(card) for card in pair) == 15
            )
        
            ranks = [card.rank for card in hand]  # calculate pairs
            pairs = len(ranks) - len(set(ranks))  # count duplicate ranks
        
            sorted_hand = sorted(card.rank for card in hand) # calculate runs (check runs of length 3 to 5)
            runs = 0
            for run_length in range(3, 6):
                for combo in combinations(sorted_hand, run_length):
                    if list(combo) == list(range(min(combo), max(combo) + 1)):
                        runs += 1
            return fifteen_combinations, pairs, runs

        def discard_to_crib(hand, best_cards=False, safer_cards=False): # function to discard high-value cards to the crib
            if best_cards:
                return hand[:4], hand[4:]  # keep best four cards
            if safer_cards:
                return hand[-2:], hand[:-2]  # discard two worst cards
            return hand, []  

        def discard_high_or_low_cards(hand): # function to discard low or high cards
            return hand[:4], hand[4:]  # keep first 4, discard the rest

        def avoid_discarding(hand, cards_to_avoid): # helper function to avoid discarding valuable cards to the opponent's crib
            safe_cards = [card for card in hand if card.rank not in cards_to_avoid]
            return safe_cards[:4], safe_cards[4:]  # keep first 4 safe cards

        def has_vulnerable_cards(hand): # check for vulnerable cards like 5, 10s, face cards
            return any(card.rank in [5, 10] for card in hand)
        
        def has_flush_opportunity(hand): # check for flush opportunities
            suits = [card.suit for card in hand]  # get the suits
            return len(set(suits)) == 1  # true if all cards have the same suit

        if am_dealer: # main decision based on whether the player has the crib or not
            fifteen_combinations, pairs, runs = calculate_combinations(hand) # strategy: maximize points in hand and crib
            if fifteen_combinations > 0 or pairs > 0 or runs > 0:
                keep, throw = discard_to_crib(hand, best_cards=True)
            else:
                keep, throw = discard_high_or_low_cards(hand)
        else:
            if has_vulnerable_cards(hand): # defensive strategy: minimize points given to opponent's crib
                keep, throw = avoid_discarding(hand, cards_to_avoid=[5, 10])
            else:
                keep, throw = discard_high_or_low_cards(hand)
                
            if has_flush_opportunity(hand): # avoid helping the opponent get a flush
                keep, throw = avoid_discarding(hand, cards_to_avoid=['flush'])

        return keep, throw, scores, am_dealer # return the kept cards, the thrown cards, the scores, and the dealer status


    def peg(self, hand, pegging_history, scores, am_dealer):

        opponent_last_play = pegging_history._prev_play # extract opponent's last play and current count from pegging history
        current_count = pegging_history._total
        
        def card_value(card): # function to get the value of a card
            if card.rank > 10:  # face cards are worth 10 points
                return 10
            return card.rank  # otherwise, the value is the rank of the card

        def can_make_15_or_31(card, current_count): # check if playing a card would make a 15 or 31
            new_count = current_count + card_value(card)
            if new_count == 15 or new_count == 31:
                return True
            return False

        def can_form_pair(card, opponent_card): # check if playing a card would form a pair
            return card.rank == opponent_card.rank

        def can_form_run(card, opponent_card): # check if playing a card would form a run (sequence)
            return abs(card.rank - opponent_card.rank) == 1

        def can_play_without_exceeding_31(card, current_count): # check if we can safely play without exceeding 31
            return current_count + card_value(card) <= 31

        for card in hand: # prioritize play decisions based on optimal strategy
            if can_make_15_or_31(card, current_count): # if 15 or 31 possible
                return card  # play card to score 2 points for 15 or 31

            if opponent_last_play and can_form_pair(card, opponent_last_play): # try to form pair with the opponent's last play
                return card  # play card to score 2 points for pair 

            if opponent_last_play and can_form_run(card, opponent_last_play): # try to form a sequence (run) with opponent's last play
                return card  # play card to score for a run

        for card in sorted(hand, key=card_value): # play lowest card possible without exceeding 31 to avoid giving opponent points
            if can_play_without_exceeding_31(card, current_count):
                return card  # play card to stay under 31 and avoid a "Go"
            
        return None # if no card can be played, return None



    

                                    
