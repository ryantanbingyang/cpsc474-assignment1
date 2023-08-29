package edu.yale.cs.cpsc474;

import java.util.List;

/**
 * A policy composed of a KeepPolicy and a PegPolicy.
 */

public class CompoundPolicy implements CribbagePolicy
{
    private KeepPolicy keep;
    private PegPolicy peg;
    
    public CompoundPolicy(KeepPolicy keep, PegPolicy peg)
    {
	this.keep = keep;
	this.peg = peg;
    }

    @Override
    public CribbageHand[] keep(CribbageHand cards, int[] scores, boolean amDealer)
    {
	return keep.keep(cards, scores, amDealer);
    }

    @Override
    public CribbageCard peg(CribbageHand cards, PeggingHistory hist, CribbageCard turn, int[] scores, boolean amDealer)
    {
	return peg.peg(cards, hist, turn, scores, amDealer);
    }
}
