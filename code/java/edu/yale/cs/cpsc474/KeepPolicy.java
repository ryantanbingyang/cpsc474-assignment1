package edu.yale.cs.cpsc474;

import java.util.List;

public interface KeepPolicy
{
    CribbageHand[] keep(CribbageHand cards, int[] scores, boolean amDealer);
}

