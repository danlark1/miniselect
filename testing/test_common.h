/*          Copyright Danila Kutenin, 2020-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
#pragma once

#include <gtest/gtest.h>

#include <algorithm>

#include "miniselect/floyd_rivest_select.h"
#include "miniselect/heap_select.h"
#include "miniselect/median_of_3_random.h"
#include "miniselect/median_of_medians.h"
#include "miniselect/median_of_ninthers.h"
#include "miniselect/pdqselect.h"

namespace miniselect {
namespace algorithms {

struct STD {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    std::partial_sort(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    std::partial_sort(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    std::nth_element(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    std::nth_element(begin, mid, end);
  }
};

struct PDQ {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    pdqpartial_sort(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    pdqpartial_sort(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    pdqselect(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    pdqselect(begin, mid, end);
  }
};

struct PDQBranchless {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    pdqpartial_sort_branchless(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    pdqpartial_sort_branchless(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    pdqselect_branchless(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    pdqselect_branchless(begin, mid, end);
  }
};

struct FloydRivest {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    floyd_rivest_partial_sort(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    floyd_rivest_partial_sort(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    floyd_rivest_select(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    floyd_rivest_select(begin, mid, end);
  }
};

struct MedianOfNinthers {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    median_of_ninthers_partial_sort(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    median_of_ninthers_partial_sort(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    median_of_ninthers_select(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    median_of_ninthers_select(begin, mid, end);
  }
};

struct MedianOfMedians {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    median_of_medians_partial_sort(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    median_of_medians_partial_sort(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    median_of_medians_select(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    median_of_medians_select(begin, mid, end);
  }
};

struct MedianOf3Random {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    median_of_3_random_partial_sort(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    median_of_3_random_partial_sort(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    median_of_3_random_select(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    median_of_3_random_select(begin, mid, end);
  }
};

struct Heap {
  template <class Iter, class Compare>
  static void Sort(Iter begin, Iter mid, Iter end, Compare&& comp) {
    heap_partial_sort(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Sort(Iter begin, Iter mid, Iter end) {
    heap_partial_sort(begin, mid, end);
  }

  template <class Iter, class Compare>
  static void Select(Iter begin, Iter mid, Iter end, Compare&& comp) {
    heap_select(begin, mid, end, std::move(comp));
  }

  template <class Iter>
  static void Select(Iter begin, Iter mid, Iter end) {
    heap_select(begin, mid, end);
  }
};

using All =
    ::testing::Types<STD, PDQ, PDQBranchless, FloydRivest, MedianOfNinthers,
                     MedianOfMedians, MedianOf3Random, Heap>;

template <class Integral>
struct IntegralCharIterator {
  using difference_type = Integral;
  using value_type = char;
  using pointer = char*;
  using reference = char&;
  using iterator_category = std::random_access_iterator_tag;

  IntegralCharIterator() = default;
  IntegralCharIterator(const IntegralCharIterator& other) : pos(other.pos) {}
  IntegralCharIterator(IntegralCharIterator&& other) : pos(other.pos) {}
  IntegralCharIterator& operator=(const IntegralCharIterator& other) {
    pos = other.pos;
    return *this;
  }
  IntegralCharIterator& operator=(IntegralCharIterator&& other) {
    pos = other.pos;
    return *this;
  }
  ~IntegralCharIterator() = default;
  IntegralCharIterator(pointer p) : pos(p) {}

  IntegralCharIterator& operator+=(difference_type other) {
    pos += other;
    return *this;
  }

  IntegralCharIterator& operator-=(difference_type other) {
    pos -= other;
    return *this;
  }

  value_type& operator[](difference_type other) {
    return pos[static_cast<size_t>(other)];
  }

  value_type& operator[](difference_type other) const {
    return pos[static_cast<size_t>(other)];
  }

  IntegralCharIterator& operator++() {
    ++pos;
    return *this;
  }

  IntegralCharIterator operator++(int) { return IntegralCharIterator(pos++); }

  IntegralCharIterator& operator--() {
    --pos;
    return *this;
  }

  IntegralCharIterator operator--(int) { return IntegralCharIterator(pos--); }

  value_type& operator*() { return *pos; }

  value_type& operator*() const { return *pos; }

  difference_type operator-(const IntegralCharIterator& other) const {
    return pos - other.pos;
  }

  IntegralCharIterator operator-(difference_type other) const {
    return IntegralCharIterator(pos - other);
  }

  IntegralCharIterator operator+(difference_type other) const {
    return IntegralCharIterator(pos + other);
  }

  bool operator==(const IntegralCharIterator& other) const {
    return pos == other.pos;
  }

  bool operator!=(const IntegralCharIterator& other) const {
    return pos != other.pos;
  }

  bool operator<(const IntegralCharIterator& other) const {
    return pos < other.pos;
  }

  bool operator>(const IntegralCharIterator& other) const {
    return pos > other.pos;
  }

  bool operator<=(const IntegralCharIterator& other) const {
    return pos <= other.pos;
  }

  bool operator>=(const IntegralCharIterator& other) const {
    return pos >= other.pos;
  }

  char* pos;
};

template <class Integral>
inline IntegralCharIterator<Integral> operator+(
    typename IntegralCharIterator<Integral>::difference_type diff,
    const IntegralCharIterator<Integral> iter) {
  return iter + diff;
}

}  // namespace algorithms
}  // namespace miniselect
