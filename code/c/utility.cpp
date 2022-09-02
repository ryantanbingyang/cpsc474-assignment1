#include "utility.h"

#include <vector>
#include <string>

#include <cstdlib>

namespace cpsc474
{
  std::vector<std::string> split(const std::string& s, char delimiter)
  {
    std::vector<std::string> splits;
    std::string token;
    for (size_t j = 0; j < s.size(); j++)
      {
	if (s[j] == delimiter)
	  {
	    // start a new token at the delimiter
	    splits.push_back(token);
	    token = "";
	  }
	else
	  {
	    // add character to current token
	    token += s[j];
	  }
      }
    // add last string to result
    splits.push_back(token);

    return splits;
  }

  Maximizer Maximizer::operator+(int x) const
  {
    if (x > max)
      {
	return Maximizer(x);
      }
    else
      {
	return Maximizer(max);
      }
  }

  Maximizer::operator int() const
  {
    return max;
  }
}
