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
	int totalGames = 0;
	long runTime = 0;
	if (args.length > 0)
	    {
		if (args[0].equals("--time"))
		    {
			try
			    {
				runTime = Integer.parseInt(args[1]) * 1000L;
				numGames = 1000;
			    }
			catch (NumberFormatException ex)
			    {
				System.err.println("TestCribbage: argument " + args[1] + " is not an integer");
				System.exit(1);
			    }
		    }
		else
		    {
			try
			    {
				numGames = Integer.parseInt(args[0]);
				runTime = 0;
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
	    }

	CribbageGame game = new CribbageGame();
	CribbagePolicy benchmark = new CompoundPolicy(new GreedyThrower(game), new GreedyPegger());
	CribbagePolicy submission = new MyPolicy(game);
	EvaluationResults eval = new EvaluationResults();
	long startTime = System.currentTimeMillis();
	while (totalGames == 0 || System.currentTimeMillis() - startTime < runTime)
	    {
		EvaluationResults results = game.evaluatePolicies(submission, benchmark, numGames);
		eval.add(results);
		totalGames += numGames;
	    }
	System.out.println("NET: " + eval.averageNet());
	System.out.println("CONF: " + (eval.averageNet() - 2 * eval.stddev()));
	System.out.println(eval);
    }
}
