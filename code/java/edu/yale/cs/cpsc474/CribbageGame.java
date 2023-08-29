package edu.yale.cs.cpsc474;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Collectors;

public class CribbageGame
{
    private int pairValue = 2;
    private int keepCards = 4;
    private int throwCards = 2;
    private int scoringSum = 15;
    private int sumValue = 2;
    private int dealCards = keepCards + throwCards;
    private int peggingLimit = 31;
    private int winningScore = 121;
    private int heelsValue = 2;

    private OrderedRanks cardRanks;
    private CharacterSuits cardSuits;
    private List<Integer> allValues;
    private List<CribbageAction> allDiscards;
    
    public CribbageGame()
    {
	cardRanks = new OrderedRanks(new String[] {"A", "2", "3", "4", "5", "6", "7", "8", "9", "T,10", "J", "Q", "K"});
	cardSuits = new CharacterSuits("SHDC");
	allValues = new ArrayList<>();
	for (int i = 1; i <= 10; i++)
	    {
		allValues.add(i);
	    }
	allDiscards = new ArrayList<>();
	for (int first = 0; first < keepCards + throwCards; first++)
	    {
		for (int second = first + 1; second < keepCards + throwCards; second++)
		    {
			List<Integer> discard = new ArrayList<>();
			discard.add(first);
			discard.add(second);
			allDiscards.add(new CribbageAction(discard));
		    }
	    }
    }
	
    
    public CribbageHand[] deal()
    {
	Deck<CardRank, Character, OrderedRanks, CharacterSuits> deck = new Deck<>(cardRanks, cardSuits, 1);
	deck.shuffle();
	List<CribbageCard> bothHandsAndTurn = deck.deal(dealCards * 2 + 1).stream().map(c -> new CribbageCard(c)).collect(Collectors.toList());
	CribbageHand p0Hand = new CribbageHand(bothHandsAndTurn.subList(0, dealCards));
	CribbageHand p1Hand = new CribbageHand(bothHandsAndTurn.subList(dealCards, dealCards * 2));
	CribbageHand turnCard = new CribbageHand(bothHandsAndTurn.subList(2 * dealCards, 2 * dealCards + 1));

	return new CribbageHand[] {p0Hand, p1Hand, turnCard};
    }

    public int cardsToKeep()
    {
	return keepCards;
    }

    public int cardsDealt()
    {
	return dealCards;
    }
    
    public int getPeggingLimit()
    {
	return peggingLimit;
    }
    
    public int sumScore(int sum)
    {
	return sum == scoringSum ? sumValue : 0;
    }

    public int pegPairValue(int count)
    {
	return pairValue * (int)Combinatorics.combinations(count, 2);
    }

    public int pegStraightValue(int length)
    {
	return length >= 3 ? length : 0;
    }

    public int pegSumValue(int sum)
    {
	return sum == 15 ? 2 : 0;
    }

    public int pegExactValue(boolean alreadyGo)
    {
	return alreadyGo ? 1 : 2;
    }
    
    /**
     * Returns the pegging point value of the given rank in this game.
     *
     * @param rank a CardRank in this game
     * @return the pegging point value of rank
     */
    public int rankValue(CardRank r)
    {
	return r.intValue() < 10 ? r.intValue() + 1 : 10;
    }

    public int handFlushValue(int length)
    {
	if (length == keepCards)
	    {
		return keepCards;
	    }
	else
	    {
		return 0;
	    }
    }

    public int turnFlushValue(int length)
    {
	if (length == keepCards + 1)
	    {
		return keepCards + 1;
	    }
	else
	    {
		return 0;
	    }
    }	
	       
    public int runValue(int length, int combinations)
    {
	if (length >= 3)
	    {
		return length * combinations;
	    }
	else
	    {
		return 0;
	    }
    }

    public int turnCardValue(CribbageCard turn)
    {
	if (turn.getRank().getName().equals("J"))
	    {
		return heelsValue;
	    }
	else
	    {
		return 0;
	    }
    }
    
    public int nobValue(CardRank r)
    {
	if (r.getName().equals("J"))
	    {
		return 1;
	    }
	else
	    {
		return 0;
	    }
    }
    
    /**
     * Returns the score and subscores that would be earned by the giv<en
     * hand with the given turn (cut) card.  The scores are returned
     * in a vector in the order total, pairs, 15s, runs, flushes, nobs.
     *
     * @param hand a CribbageHand, non-null
     * @param turn a CribbageCard, or null
     * @param cribRules true to score using crib scoring rules, false otherwise
     * @return an array of integers where the first element is the total
     * of the remaining elements
     */
    public int[] score(CribbageHand hand, CribbageCard turn, boolean cribRules)
    {
	// make one hand with turn card if given
	CribbageHand allCards = turn == null ? hand : new CribbageHand(hand, new CribbageHand(Arrays.asList(new CribbageCard[] {turn})));

	Map<Integer, Integer> valueCount = new HashMap<>();
	for (Integer i : allValues)
	    {
		valueCount.put(i, 0);
	    }
	Map<CardRank, Integer> rankCount = new HashMap<>();
	for (CardRank r : cardRanks.allRanks())
	    {
		rankCount.put(r, 0);
	    }
	Map<Character, Integer> suitCount = new HashMap<>();
	for (Character s : cardSuits.allSuits())
	    {
		suitCount.put(s, 0);
	    }
	
	for (CribbageCard c : allCards)
	    {
		CardRank rank = c.getRank();
		char suit = c.getSuit();
		int value = rankValue(rank);
		valueCount.put(value, valueCount.get(value) + 1);
		rankCount.put(rank, rankCount.get(rank) + 1);
		suitCount.put(suit, suitCount.get(suit) + 1);
	    }

	int fifteens = 0;
	for (int count = 2; count <= allCards.size(); count++)
	    {
		for (List<CribbageCard> subset : IterTools.combinations(allCards, count))
		    {
			int sum = 0;
			for (CribbageCard c : subset)
			    {
				sum += rankValue(c.getRank());
			    }
			fifteens += sumScore(sum);
		    }
	    }

	int pairs = 0;
	for (CardRank r : cardRanks.allRanks())
	    {
		pairs += Combinatorics.combinations(rankCount.get(r), 2);
	    }
	pairs *= pairValue;

	int runs = 0;
	int currRun = 0;
	int combinations = 1;
	for (CardRank r : cardRanks.allRanks())
	    {
		if (rankCount.get(r) == 0)
		    {
			runs += runValue(currRun, combinations);
			currRun = 0;
			combinations = 1;
		    }
		else
		    {
			currRun += 1;
			combinations *= rankCount.get(r);
		    }
	    }
	runs += runValue(currRun, combinations);

	int flushes = 0;
	int maxFlush = 0;
	char longSuit = '0';
	for (Character s : cardSuits.allSuits())
	    {
		if (suitCount.get(s) > maxFlush)
		    {
			longSuit = s;
			maxFlush = suitCount.get(s);
		    }
	    }
	if (maxFlush == hand.size() && !cribRules && (turn == null || turn.getSuit() != longSuit))
	    {
		flushes += handFlushValue(hand.size());
	    }
	else if (maxFlush == hand.size() + 1)
	    {
		flushes += turnFlushValue(maxFlush);
	    }

	int nobs = 0;
	for (CribbageCard c : hand)
	    {
		if (turn != null && c.getSuit() == turn.getSuit())
		    {
			nobs += nobValue(c.getRank());
		    }
	    }
	
	return new int[] {pairs + fifteens + runs + flushes + nobs, pairs, fifteens, runs, flushes, nobs};
    }
    
    public CribbageCard parseCard(String card)
    {
	CardRank rank = cardRanks.parseRank(card.substring(0, card.length() - 1));
	Character suit = cardSuits.parseSuit(card.substring(card.length() - 1));
	return new CribbageCard(rank, suit);
    }

    public int[] play(CribbagePolicy p0, CribbagePolicy p1, Logger logger)
    {
	int[] scores = new int[] {0, 0};
	CribbagePolicy[] policies = new CribbagePolicy[] {p0, p1};
	int dealer = 0;
	int handsPlayed = 0;

	while (MoreArrays.max(scores) < winningScore)
	    {
		logger.log("Dealing at " + Arrays.toString(scores));
		// deal cards
		CribbageHand[] cardsInPlay = deal();
		handsPlayed++;
		// turned card is first (only) element of third part of deal
		CribbageCard turn = cardsInPlay[2].iterator().next();
		logger.log("TURN: " + turn);
		
		// check for 2 for heels (turned card is a Jack)
		int heels = turnCardValue(turn);
		scores[dealer] += heels;
		if (heels != 0)
		    {
			logger.log(Arrays.toString(scores));
		    }

		// get keep/throw for each player
		CribbageHand[][] keeps = new CribbageHand[2][];
		for (int p = 0; p < 2; p++)
		    {
			keeps[p] = policies[p].keep(cardsInPlay[p], scores, p == dealer);
			if (!cardsInPlay[p].isLegalSplit(keeps[p]))
			    {
				throw new RuntimeException(Arrays.toString(keeps[p]) + " does not partition " + cardsInPlay[p]);
			    }
			else if (keeps[p][0].size() != keepCards)
			    {
				throw new RuntimeException("Invalid partition sizes " + Arrays.toString(keeps[p]));
			    }
		    }

		// initialize pegging
		int pegTurn = 1 - dealer;
		PeggingHistory history = new PeggingHistory(CribbageGame.this);
		CribbageHand[] pegCards = new CribbageHand[] {keeps[0][0], keeps[1][0]};
		while (MoreArrays.max(scores) < winningScore
		       && !history.isTerminal())
		    {
			// get player's played card
			CribbageCard play = policies[pegTurn].peg(pegCards[pegTurn], history, turn, pegTurn == 0 ? Arrays.copyOf(scores, scores.length) : MoreArrays.reverse(scores), pegTurn == dealer);

			// check for legality of chosen card
			if (play == null && history.hasLegalPlay(pegCards[pegTurn], pegTurn == dealer ? 0 : 1))
			    {
				throw new RuntimeException("passing when " + pegCards[pegTurn] + " contains a valid card");
			    }
			else if (play != null && !history.isLegal(play, pegTurn == dealer ? 0 : 1))
			    {
				throw new RuntimeException("chosen card " + play + " us not legal");
			    }
			
			history = history.play(play, pegTurn == dealer ? 0 : 1);

			logger.log(pegTurn + " " + play);
			
			// score the play
			int[] playScore = history.getScore();
			if (playScore[0] > 0)
			    {
				scores[pegTurn] += playScore[0];
				logger.log(Arrays.toString(scores));
			    }
			else if (playScore[0] < 0)
			    {
				scores[1 - pegTurn] += -playScore[0];
				logger.log(Arrays.toString(scores));
			    }

			// remove played card from hand
			if (play != null)
			    {
				CribbageHand newHand = pegCards[pegTurn].remove(play);
				if (newHand == null)
				    {
					throw new RuntimeException("played card " + play + " not in hand " + pegCards[pegTurn]);
				    }
				pegCards[pegTurn] = newHand;
			    }

			// next player's turn
			pegTurn = 1 - pegTurn;
		    }

		// score non-dealer's hand
		if (MoreArrays.max(scores) < winningScore)
		    {
			int[] handScore = score(keeps[1 - dealer][0], turn, false);
			scores[1 - dealer] += handScore[0];
			logger.log("NON-DEALER: "
				   + keeps[1 - dealer][0]
				   + " " + Arrays.toString(scores));
		    }

		// score dealer's hand
		if (MoreArrays.max(scores) < winningScore)
		    {
			int[] handScore = score(keeps[dealer][0], turn, false);
			scores[dealer] += handScore[0];
			logger.log("DEALER: "
				   + keeps[dealer][0]
				   + " " + Arrays.toString(scores));
		    }

		// score crib
		if (MoreArrays.max(scores) < winningScore)
		    {
			CribbageHand crib = new CribbageHand(keeps[0][1], keeps[1][1]);
			int[] handScore = score(crib, turn, true);
			scores[dealer] += handScore[0];
			logger.log("CRIB: "
				   + crib
				   + " " + Arrays.toString(scores));
		    }

		// change dealer
		dealer = 1 - dealer;
	    }
	logger.log(Arrays.toString(scores));

	return new int[] {gameValue(scores), handsPlayed};
    }

    public EvaluationResults evaluatePolicies(CribbagePolicy p0, CribbagePolicy p1, int count)
    {
	Logger nullLogger = new NullLogger();
	EvaluationResults results = new EvaluationResults();
	
	for (int g = 0; g < count; g++)
	    {
		int points;
		int[] result;
		if (g % 2 == 0)
		    {
			result = play(p0, p1, nullLogger);
			points = result[0];
		    }
		else
		    {
			result = play(p1, p0, nullLogger);
			points = -result[0];
		    }
		results.update(points, result[1]);
	    }

	return results;
    }

    public static double computeMean(Map<Integer, Integer> results)
    {
	int totalGames = 0;
	int netPoints = 0;
	for (Map.Entry<Integer, Integer> e: results.entrySet())
	    {
		int points = e.getKey();
		int games = e.getValue();
		totalGames += games;
		netPoints += points * games;
	    }
	return netPoints / (double)totalGames;
    }
    
    public int gameValue(int[] scores)
    {
	if (MoreArrays.max(scores) < winningScore)
	    {
		return 0;
	    }

	int points = 1;
	int loserScore = MoreArrays.min(scores);
	if (loserScore <= 60)
	    {
		points = 3;
	    }
	else if (loserScore <= 90)
	    {
		points = 2;
	    }

	return (scores[0] > scores[1] ? 1 : -1) * points;
    }

    public class CribbageState
    {
	/** The scores for the two players, dealer in index 0 */
	private int[] scores;

	/**
	 * Two-element array with dealer's hand and other player's hand.  Nulls if no cards dealt.
	 */
	private CribbageHand[] hands;

	/**
	 * The card turned up during the cut.  Note that thus is set during the initial deal but does not
	 * affect the information sets until after both discards.
	 */
	private CribbageCard turn;
	
	/**
	 * The cards discarded by each player.  A null element indicates no discard yet.  If the crib has
	 * been scored then there is one element giving the crib (the two players' discards combined).
	 */
	private CribbageHand[] discards;

	
	/**
	 * The cards remaining in each player's hand during ther play phase.
	 */
	private CribbageHand[] playable;
	
	/**
	 * The prgging history, or null before the pegging phase.
	 */
	private PeggingHistory history;

	/**
	 * Thenext player to play during the play phase.
	 */
	private int player;
	
	/**
	 * Creates the state at the beginning of the game -- no cards dealt and score 0-0.
	 */
	public CribbageState()
	{
	    this(new int[] {0, 0});
	}

	/**
	 * Creates the state at the beginning of a hand with the given score.
	 *
	 * @param s a two-element array of non-negative integers giving the scores of the two players, dealyer first
	 */
	public CribbageState(int[] s)
	{
	    this(s,
		 new CribbageHand[] {null, null, null},
		 null,
		 new CribbageHand[] {null, null},
		 new CribbageHand[] {null, null},
		 null,
		 1);
	}

	private CribbageState(int[] s, CribbageHand[] h, CribbageCard t, CribbageHand[] d, CribbageHand[] p, PeggingHistory his, int toPlay)
	{
	    scores = new int[] {s[0], s[1]};
	    hands = h;
	    turn = t;
	    discards = d;
	    playable = p;
	    history = his;
	    player = toPlay;
	}
	    
	
	/**
	 * Determines if this state represents the end of the game or the end of the hand.
	 *
	 * @return true if and only if this state is the end of the game or the end of the hand
	 */
	public boolean isTerminal()
	{
	    // terminal if either player has winning score or if at end of hand
	    return scores[0] >= winningScore || scores[1] >= winningScore || (discards != null && discards.length == 1);
	}


	/**
	 * Returns the scores in this state, dealer at the beginning of the gahdn first.
	 *
	 * @return the scores in this state
	 */
	public int[] getScores()
	{
	    return new int[] {scores[0], scores[1]};
	}

	
	/**
	 * Returns the list of actionas available to the current player in this state.  If this state
	 * is terminal or if this state is a chance state, then the return value is null.  If the only
	 * legal action is to pass, the return value is a one-element list containing null.  Otherwise,
	 * the return value is a list of legal actions.
	 */
	public List<GameAction> getActions()
	{
	    if (isTerminal())
		{
		    // hand over
		    return null;
		}
	    else if (hands[0] == null)
		{
		    // deal hands
		    return null;
		}
	    else if (discards[0] == null || discards[1] == null)
		{
		    // current player chooses discards
		    return new ArrayList<GameAction>(allDiscards);
		}
	    else if (playable[0].size() + playable[1].size() > 0)
		{
		    // determine legal plays for current player
		    List<GameAction> actions = new ArrayList<>();
		    int index = 0;
		    for (CribbageCard c : playable[player])
			{
			    if (history.isLegal(c, player))
				{
				    List<Integer> play = new ArrayList<>();
				    play.add(index);
				    actions.add(new CribbageAction(play));
				}
			    index++;
			}
		    if (actions.size() == 0)
			{
			    actions.add(null);
			}
		    return actions;
		}
	    else
		{
		    // count at end of hand
		    List<GameAction> actions = new ArrayList<>();
		    actions.add(null);
		    return actions;
		}
	}

	
	/**
	 * Returns the state that results from the given action, provided that the current state is not
	 * terminal.
	 *
	 * @param a one of the GameActions returned by the getActions method for this state
	 * @return the new state
	 */
	public CribbageState nextState(GameAction a)
	{
	    if (hands[0] == null)
		{
		    // initial deal -- action must be null
		    if (a != null)
			{
			    throw new IllegalArgumentException("action for initial deal non-null: " + a);
			}
		    CribbageHand[] d = deal();
		    return new CribbageState(scores,
					     new CribbageHand[] {d[0], d[1]},
					     d[2].iterator().next(),
					     new CribbageHand[] {null, null},
					     new CribbageHand[] {null, null},
					     null,
					     player);
		}
	    else if (hands[0].size() > keepCards)
		{
		    // dealer discard
		    CribbageHand[] result = hands[0].split(((CribbageAction)a).getDiscard());
		    return new CribbageState(scores,
					     new CribbageHand[] {result[0], hands[1]},
					     turn,
					     new CribbageHand[] {result[1], null},
					     new CribbageHand[] {result[0], null},
					     null,
					     player);
		}
	    else if (hands[1].size() > keepCards)
		{
		    // non-dealer discard
		    CribbageHand[] result = hands[1].split(((CribbageAction)a).getDiscard());

		    // new state accounts for heels
		    return new CribbageState(new int[] {scores[0] + turnCardValue(turn), scores[1]},
					     new CribbageHand[] {hands[0], result[0]},
					     turn,
					     new CribbageHand[] {discards[0], result[1]},
					     new CribbageHand[] {playable[0], result[0]},
					     new PeggingHistory(CribbageGame.this),
					     player);
		}
	    else if (playable[0].size() + playable[1].size() > 0)
		{
		    // play a card
		    CribbageCard play = null;
		    CribbageHand[] newPlayable = {playable[0], playable[1]};
		    if (a != null)
			{
			    // play a card
			    CribbageHand[] result = playable[player].split(((CribbageAction)a).getPlay());
			    newPlayable[player] = result[0];
			    play = result[1].iterator().next();
			}
		    int[] points = history.score(play, player);
		    int[] newScores = new int[] {scores[0], scores[1]};
		    if (points[0] < 0)
			{
			    newScores[1 - player] += -points[0];
			}
		    else
			{
			    newScores[player] += points[0];
			}
		    PeggingHistory newHistory = history.play(play, player);
		    return new CribbageState(newScores,
					     hands,
					     turn,
					     discards,
					     newPlayable,
					     newHistory,
					     1 - player);
		}
	    else
		{
		    int[] newScores = new int[] {scores[0], scores[1]};
		    CribbageHand crib = new CribbageHand(discards[0], discards[1]);
		    
		    // score non-dealer's hand
		    newScores[1] += score(hands[1], turn, false)[0];
		    
		    // score dealer's hand
		    if (newScores[1] < winningScore)
			{
			    newScores[0] += score(hands[0], turn, false)[0];

			    if (newScores[0] < winningScore)
				{
				    // score crib
				    newScores[0] += score(crib, turn, true)[0];
				}
			}
		    
		    return new CribbageState(newScores,
					     hands,
					     turn,
					     new CribbageHand[] {crib},
					     playable,
					     history,
					     1 - player);
		}
	}

	
	public String toString()
	{
	    return Arrays.toString(scores) + Arrays.toString(hands) + turn + Arrays.toString(discards) + Arrays.toString(playable) + history + player;
	}
    }

    
    private class CribbageAction implements GameAction
    {
	/* A list of lists of integers of valid card combinations to play.  For discard actions,
	 * that is a list of two indices, one for each 2-combination of cards in the hand.  For
	 * pegging actions, that is a list of one index giving the index of a legal card to play.
	 */
	private List<Integer> cardCombos;

	
	public CribbageAction(List<Integer> action)
	{
	    cardCombos = new ArrayList<>(action);
	}

	
	public List<Integer> getDiscard()
	{
	    return cardCombos;
	}

	
	public List<Integer> getPlay()
	{
	    return cardCombos;
	}
    }

    public static void main(String[] args)
    {
	CribbageGame g = new CribbageGame();
	int[] scores = new int[] {0, 0};
	int dealer = 0;
	while (scores[0] < g.winningScore && scores[1] < g.winningScore)
	    {
		CribbageGame.CribbageState s = g.new CribbageState(scores);
		while (!s.isTerminal())
		    {
			List<GameAction> actions = s.getActions();
			if (actions == null)
			    {
				s = s.nextState(null);
			    }
			else
			    {
				s = s.nextState(actions.get((int)(Math.random() * actions.size())));
			    }
			System.out.println(s);
		    }
		System.out.println(s);
		scores = s.getScores();

		// switch who is dealer
		dealer = 1 - dealer;
		scores = new int[] {scores[1], scores[0]};
	    }
    }
}

