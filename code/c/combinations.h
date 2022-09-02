#ifndef __COMBINATAIONS_H__
#define __COMBINATAIONS_H__

#include <vector>

namespace cpsc474
{
  /**
   * An object representing the combinations (subsets) of a given list of items.
   *
   * @param T an assignable type
   */
  template<class T>
    class Combinations
    {
    public:
      class iterator;

      /**
       * Creates an object representing the size-n combinations (subsets) of the given
       * items.  Any duplicate items are treated as distinct.
       *
       * @param items a list of items
       * @param n a nonnegative integer not greater than the size of items
       */
      Combinations(const std::vector<T>& items, size_t n);

      /**
       * Returns an iterator positioned at the first combination of n items.
       *
       * @return an iterator positioned at the first combination of n items
       */
      iterator begin() const;

      /**
       * Returns an iterator that has completed a pass through all size-n combinations.
       *
       * @return an iterator that has completed a pass through all size-n combinations
       */
      iterator end() const;

      /**
       * An iterator over all the combinations of a given size.
       */
      class iterator
      {
	friend class Combinations;
      private:
	/**
	 * Returns an iterator that has completed a pass through all combinations of
	 * the given size.
	 *
	 * @return an iterator that has completed a pass through all combinations of
	 * the given size
	 */
	iterator(const std::vector<T>& items);

	/**
	 * Returns an iterator positioned at the first combination of the given size.
	 *
	 * @param items a vector of items
	 * @param size a nonnegative integer not greater than the number of items
	 * @return an iterator positioned at the first combination of the given size
	 */
	iterator(const std::vector<T>& items, size_t size);

      public:
	/**
	 * Advances this iterator to the next combination.  The result is undefined
	 * if this iterator has completed a pass through all combinations of the given size
	 *
	 * @return a reference to this iterator
	 */
	iterator& operator++();

	/**
	 * Returns the combination this iterator is currently positioned at.
	 *
	 * @return the combination this iterator is currently positioned at
	 */
	const std::vector<T>& operator*() const;

	/**
	 * Determines if the given iterator is positioned at the same combination as
	 * this one.
	 *
	 * @param it an iterator over the same items as this one
	 * @return true if the iterators are positioned at the same combination or
	 * have both completed a pass over the combinations of those items of the given
	 * size; false otherwise
	 */
	bool operator==(const iterator& it) const { return indices == it.indices; }
	
	/**
	 * Determines if the given iterator is positioned at a different combination as
	 * this one.
	 *
	 * @param it an iterator over the same items as this one
	 * @return true if the iterators are positioned at different combinations or
	 * one and only one has completed a pass over the combinations of those
	 * items of the given size; false otherwise
	 */
	bool operator!=(const iterator& it) const { return !(*this == it); }

      private:
	/**
	 * The items to iterate over combinations of.
	 */
	const std::vector<T>& items;

	/**
	 * The size of the combinations to iterate over.
	 */
	size_t size;

	/**
	 * The indices into items of the members of the current combinations.
	 */
	std::vector<size_t> indices;

	/**
	 * The items corresponding to the current subset.  This is meaningful only
	 * when thiss iterator has not completed a pass.
	 */
	std::vector<T> subset;
      };
      
    private:
	/**
	 * The items to iterate over combinations of.
	 */
	const std::vector<T>& items;

	/**
	 * The size of the combinations to iterate over.
	 */
	size_t size;
    };
}

#include "combinations.cpp"

#endif

