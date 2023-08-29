package edu.yale.cs.cpsc474;

import java.util.List;

public interface PegPolicy
{
    CribbageCard peg(CribbageHand cards, PeggingHistory hist, CribbageCard turn, int[] scores, boolean amDealer);
}

