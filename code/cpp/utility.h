#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <vector>
#include <string>

#include <climits>

namespace cpsc474
{
  std::vector<std::string> split(const std::string& s, char delimiter);

  class Maximizer
  {
  public:
  Maximizer(int init = INT_MIN) : max(init) {}
    Maximizer operator+(int x) const;
    operator int() const;

  private:
    int max;
  };

  template <class P, class T>
  class PointerHash
    {
    public:
      size_t operator()(const P& o) const noexcept { return std::hash<T>()(*o); }
    };

  template <class P, class T>
    class PointerDeepEquals
    {
    public:
      bool operator()(const P& o1, const P& o2) const noexcept { return *o1 == *o2; }
    };
}

#endif
