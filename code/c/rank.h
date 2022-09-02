#ifndef __RANK_H__
#define __RANK_H__

#include <string>
#include <ostream>

namespace cpsc474
{

  class CardRank
  {
  public:
    CardRank(int o, const std::string& n);

    int intValue() const;
    
    const std::string& getName() const;
    
    bool operator==(const CardRank& other) const;
    
    bool operator<(const CardRank& other) const;
    
    void print(std::ostream& os) const;

  private:
    int ordinal;
    std::string name;
  };

  std::ostream& operator<<(std::ostream& os, const CardRank& rank);
}

namespace std
{
  
  template<>
    class hash<cpsc474::CardRank>
    {
    public:
      size_t operator()(const cpsc474::CardRank& rank) const { return rank.intValue(); }
    };
    
}

#endif
