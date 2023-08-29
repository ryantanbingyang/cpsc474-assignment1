#include <iostream>
#include <tuple>

#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "game.h"
#include "policy.h"
#include "greedy.h"
#include "my_policy.h"

using cpsc474::CribbageGame;
using cpsc474::CompoundPolicy;
using cpsc474::GreedyThrower;
using cpsc474::GreedyPegger;
using cpsc474::MyPolicy;
using cpsc474::EvaluationResults;

void reportResults(const EvaluationResults& results);

int main(int argc, char **argv)
{
  size_t numGames = 2;
  size_t totalGames = 0;
  long runTime = 0;
  
  if (argc > 1)
    {
      if (strcmp(argv[1], "--time") == 0)
	{
	  numGames = 1000;
	  runTime = atoi(argv[2]);
	}
      else
	{
	  runTime = 0;
	  numGames = atoi(argv[1]);
	}
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
  MyPolicy submission(game);
  
  EvaluationResults results;

  try
    {
      time_t startTime = time(NULL);
      while (totalGames == 0 || time(NULL) - startTime < runTime)
	{
	  EvaluationResults batchResults = game.evaluatePolicies(&submission, &benchmark, (size_t)numGames);
	  results = cpsc474::add(results, batchResults);
	  totalGames += numGames;
	}
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
	    << std::endl;
  long sumSquares = 0;
  long sumValues = 0;
  for (auto i = std::get<1>(results).begin(); i != std::get<1>(results).end(); i++)
    {
      sumSquares += i->first * i->first * i->second;
      sumValues += i->first * i->second;
    }
  int numGames = std::get<3>(results);
  double mean = (double)sumValues / numGames;
  double variance = (double)sumSquares / numGames - mean * mean;
  double stddev = sqrt(variance) / sqrt(numGames);
  std::cout << "CONF: " << mean - 2 * stddev << std::endl;
  std::cout << std::get<0>(results).first
	    << "-" << std::get<0>(results).second
	    << " {";
  for (auto i = std::get<1>(results).begin(); i != std::get<1>(results).end(); i++)
    {
      std::cout << (*i).first << "=" << (*i).second << " ";
    }
  std::cout << "} (" << std::get<2>(results) << " hands/game)" << std::endl;
}
