#include <string>
#include <vector>

#include <memory>

using std::shared_ptr;

#include "character_suits.h"

namespace cpsc474
{
  CharacterSuits::CharacterSuits(const std::string& s) : suitString(s)
  {
    const char *c = suitString.c_str();
    size_t count = s.size();
    for (size_t i = 0; i < count; i++)
      {
	suits.push_back(c + i);
      }
  }

  const std::vector<const char *>& CharacterSuits::allSuits() const
  {
    return suits;
  }

  const char *CharacterSuits::parseSuit(char s) const
  {
    size_t count = suits.size();
    size_t i = 0;
    while (i < count && *(suits[i]) != s)
      {
	i++;
      }
    if (i == count)
      {
	return NULL;
      }
    else
      {
	return suits[i];
      }
  }
}
