#include <vector>
#include <string>

#include "ordered_ranks.h"
#include "rank.h"
#include "utility.h"

namespace cpsc474
{
  OrderedRanks::OrderedRanks(const std::vector<std::string>& names)
  {
    size_t count = names.size();
    for (size_t i = 0; i < count; i++)
      {
	std::vector<std::string> splits = split(names[i], '|');
	CardRank *rank = new CardRank(i, splits[0]); 
	ranks.push_back(rank);
	const_ranks.push_back(rank);
	
	for (size_t a = 1; a < splits.size(); a++)
	  {
	    aliases[splits[a]] = splits[0];
	  }
      }
  }

  OrderedRanks::~OrderedRanks()
  {
    for (CardRank *r : ranks)
      {
	delete r;
      }
  }

  const std::vector<const CardRank *> OrderedRanks::allRanks() const
  {
    return const_ranks;
  }

  const CardRank * OrderedRanks::parseRank(const std::string& rank) const
  {
    const std::string& canonicalRank = aliases.count(rank) > 0 ? aliases.at(rank) : rank;
    
    size_t count = ranks.size();
    size_t i = 0;
    while (i < count && ranks[i]->getName() != canonicalRank)
      {
	i++;
      }
    if (i == count)
      {
	return NULL;
      }
    else
      {
	return ranks[i];
      }
  }
  
}
