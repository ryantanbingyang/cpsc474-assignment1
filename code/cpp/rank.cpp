#include <string>
#include <ostream>

#include "rank.h"

namespace cpsc474
{
  CardRank::CardRank(int o, const std::string& n) : ordinal(o), name(n)
  {
  }
  
  int CardRank::intValue() const
  {
    return ordinal;
  }

  const std::string& CardRank::getName() const
  {
    return name;
  }

  bool CardRank::operator==(const CardRank& other) const
  {
    return ordinal == other.ordinal && name == other.name;
  }

  bool CardRank::operator<(const CardRank& other) const
  {
    return ordinal < other.ordinal;
  }

  void CardRank::print(std::ostream& os) const
  {
    os << name;
  }
  
  std::ostream& operator<<(std::ostream& os, const CardRank& rank)
  {
    rank.print(os);
    return os;
  }

}

