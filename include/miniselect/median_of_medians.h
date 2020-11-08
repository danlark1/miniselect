/*          Copyright Danila Kutenin, 2020-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <utility>

#include "private/median_common.h"

namespace miniselect {
namespace median_of_medians_detail {

template <class Iter, class Compare>
static inline Iter partition(Iter r, Iter end, Compare comp) {
  const size_t len = end - r;
  if (len < 5) {
    return median_common_detail::pivotPartition(r, len / 2, len, comp);
  }
  size_t j = 0;
  for (size_t i = 4; i < len; i += 5, ++j) {
    median_common_detail::partition5(r, i - 4, i - 3, i, i - 2, i - 1, comp);
    std::swap(r[i], r[j]);
  }
  median_common_detail::quickselect<Iter, Compare, &partition>(r, r + j / 2,
                                                               r + j, comp);
  return median_common_detail::pivotPartition(r, j / 2, len, comp);
}

}  // namespace median_of_medians_detail

template <class Iter, class Compare>
inline void median_of_medians_select(Iter begin, Iter mid, Iter end,
                                     Compare comp) {
  if (mid == end) return;

  median_common_detail::quickselect<
      Iter, Compare, &median_of_medians_detail::partition<Iter, Compare>>(
      begin, mid, end, comp);
}

template <class Iter>
inline void median_of_medians_select(Iter begin, Iter mid, Iter end) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  median_of_medians_select(begin, mid, end, std::less<T>());
}

template <class Iter, class Compare>
inline void median_of_medians_sort(Iter begin, Iter mid, Iter end,
                                   Compare comp) {
  if (begin == mid) return;
  median_common_detail::quickselect<
      Iter, Compare, &median_of_medians_detail::partition<Iter, Compare>>(
      begin, mid - 1, end, comp);
  std::sort(begin, mid, comp);
}

template <class Iter>
inline void median_of_medians_sort(Iter begin, Iter mid, Iter end) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  median_of_medians_sort(begin, mid, end, std::less<T>());
}

}  // namespace miniselect
