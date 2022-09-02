#include <iostream>
#include <tuple>

#include <cstdlib>
#include <ctime>
#include <cstdlib>

#include "game.h"
#include "policy.h"
#include "greedy.h"
#include "c_policy.h"
#include "c_cribbage.h"

using cpsc474::CribbageGame;
using cpsc474::CompoundPolicy;
using cpsc474::CPolicy;
using cpsc474::GreedyThrower;
using cpsc474::GreedyPegger;
using cpsc474::EvaluationResults;

void reportResults(const EvaluationResults& results);

int main(int argc, char **argv)
{
  size_t numGames = 2;
  
  if (argc > 1)
    {
      numGames = atoi(argv[1]);
    }
  if (numGames <= 0)
    {
      std::cerr << argv[0] << ": number of games must be a positive integer"
		<< std::endl;
      exit(1);
    }
  
  srand(time(NULL));

  CribbageGame game;
  GreedyThrower thrower(game);
  GreedyPegger pegger(game);
  CompoundPolicy benchmark = CompoundPolicy(thrower, pegger);
  CPolicy submission(game, ::keep, ::peg);
  
  EvaluationResults results;

  try
    {
      results = game.evaluatePolicies(&submission, &benchmark, (size_t)numGames);
    }
  catch (const std::string& s)
    {
      std::cerr << s << std::endl;
      exit(1);
    }

  reportResults(results);
}

void reportResults(const EvaluationResults& results)
{
  std::cout << "NET: " << std::get<0>(results).first - std::get<0>(results).second
	    << std::endl
	    << std::get<0>(results).first
	    << "-" << std::get<0>(results).second
	    << " {";
  for (auto i = std::get<1>(results).begin(); i != std::get<1>(results).end(); i++)
    {
      std::cout << (*i).first << "=" << (*i).second << " ";
    }
  std::cout << "} (" << std::get<2>(results) << " hands/game)" << std::endl;
}
