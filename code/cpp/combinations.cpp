#include "combinations.h"

#include <vector>

namespace cpsc474
{
  template<class T>
  Combinations<T>::Combinations(const std::vector<T>& items, size_t n) : items(items), size(n)
  {
  }

  template<class T>
  typename Combinations<T>::iterator Combinations<T>::begin() const
  {
    return iterator(items, size);
  }

  template<class T>
  typename Combinations<T>::iterator Combinations<T>::end() const
  {
    return iterator(items);
  }

  template<class T>
  Combinations<T>::iterator::iterator(const std::vector<T>& items) : items(items), size(-1)
  {
    // terminal iterator is represented by [-1], which is impossible for any size
    indices.push_back(-1);
  }

  template<class T>
  Combinations<T>::iterator::iterator(const std::vector<T>& items, size_t n) : items(items), size(n), indices(n, -1), subset(n)
  {
    // inital combination is items 0 through n - 1
    for (size_t i = 0; i < n; i++)
      {
	indices[i] = i;
	subset[i] = items[i];
      }
  }

  template<class T>
  typename Combinations<T>::iterator& Combinations<T>::iterator::operator++()
  {
    // find last incrementable position
    size_t pos = size;
    while (pos > 0 && indices[pos - 1] == (size_t)(pos - 1) + (size_t)(items.size() - size))
      {
	pos--;
      }
    if (pos == 0)
      {
	// no incrementable position -- signal completion
	indices.clear();
	indices.push_back(-1);
      }
    else
      {
	// increment given position
	indices[pos - 1] += 1;
	subset[pos - 1] = items[indices[pos - 1]];
	// set later positions in sequence
	for (size_t p = pos; p < size; p++)
	  {
	    indices[p] = indices[p - 1] + 1;
	    subset[p] = items[indices[p]];
	  }
      }

    return *this;
  }

  template<class T>
  const std::vector<T>& Combinations<T>::iterator::operator*() const
  {
    return subset;
  }
  
}
