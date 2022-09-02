import edu.yale.cs.cpsc474.GreedyThrower;
import edu.yale.cs.cpsc474.GreedyPegger;
import edu.yale.cs.cpsc474.CribbagePolicy;
import edu.yale.cs.cpsc474.CompoundPolicy;
import edu.yale.cs.cpsc474.CribbageCard;
import edu.yale.cs.cpsc474.CribbageHand;
import edu.yale.cs.cpsc474.PeggingHistory;
import edu.yale.cs.cpsc474.CribbageGame;

public class MyPolicy implements CribbagePolicy
{
    // the default implementation delegates decisions to an existing policy
    private CompoundPolicy policy;
    
    public MyPolicy(CribbageGame game)
    {
	policy = new CompoundPolicy(new GreedyThrower(game), new GreedyPegger());
    }

    @Override
    public CribbageHand[] keep(CribbageHand cards, int[] scores, boolean amDealer)
    {
	return policy.keep(cards, scores, amDealer);
    }

    @Override
    public CribbageCard peg(CribbageHand cards, PeggingHistory hist, int[] scores, boolean amDealer)
    {
	return policy.peg(cards, hist, scores, amDealer);
    }
}
