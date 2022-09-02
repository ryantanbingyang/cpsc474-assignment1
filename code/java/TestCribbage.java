import edu.yale.cs.cpsc474.GreedyThrower;
import edu.yale.cs.cpsc474.GreedyPegger;
import edu.yale.cs.cpsc474.CribbagePolicy;
import edu.yale.cs.cpsc474.CompoundPolicy;
import edu.yale.cs.cpsc474.PeggingHistory;
import edu.yale.cs.cpsc474.EvaluationResults;
import edu.yale.cs.cpsc474.CribbageGame;

public class TestCribbage
{
    public static void main(String[] args)
    {
	int numGames = 2;
	if (args.length > 0)
	    {
		try
		    {
			numGames = Integer.parseInt(args[0]);
			if (numGames < 0)
			    {
				System.err.println("TestCribbage: number of games must be positive");
			    }
		    }
		catch (NumberFormatException ex)
		    {
			System.err.println("TestCribbage: argument " + args[0] + " is not an integer");
			System.exit(1);
		    }
	    }

	CribbageGame game = new CribbageGame();
	CribbagePolicy benchmark = new CompoundPolicy(new GreedyThrower(game), new GreedyPegger());
	CribbagePolicy submission = new MyPolicy(game);
	EvaluationResults results = game.evaluatePolicies(submission, benchmark, numGames);
	System.out.println("NET: " + results.averageNet());
	System.out.println(results);
    }
}
