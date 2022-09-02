#ifndef __ORDERED_RANKS__
#define __ORDERED_RANKS__

#include <vector>
#include <unordered_map>

#include "rank.h"

namespace cpsc474
{
  
  class OrderedRanks
  {
  public:
    OrderedRanks(const std::vector<std::string>& ranks);

    ~OrderedRanks();
    
    const std::vector<const CardRank *> allRanks() const;
    
    const CardRank *parseRank(const std::string& rank) const;

    const std::vector<const CardRank *>& getRanks() const { return const_ranks; }
    
    typename std::vector<const CardRank *>::const_iterator begin() const { return const_ranks.begin(); }

    typename std::vector<const CardRank *>::const_iterator end() const { return const_ranks.end(); }

  private:
    std::vector<CardRank *> ranks;
    std::vector<const CardRank *> const_ranks;
    std::unordered_map<std::string, std::string> aliases;
  };

}

#endif
