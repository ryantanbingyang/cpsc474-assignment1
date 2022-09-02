#ifndef __CHARACTER_SUITS__
#define __CHARACTER_SUITS__

#include <string>
#include <vector>

namespace cpsc474
{
  class CharacterSuits
  {
  public:
    CharacterSuits(const std::string& s);

    const std::vector<const char*>& allSuits() const;

    const char *parseSuit(char s) const;

    const std::vector<const char *>& getSuits() const { return suits; }
    
    typename std::vector<const char *>::const_iterator begin() const { return suits.begin(); }

    typename std::vector<const char *>::const_iterator end() const { return suits.end(); }
    
  private:
    std::vector<const char *> suits;
    std::string suitString;
  };
}

#endif
