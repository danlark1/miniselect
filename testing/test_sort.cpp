/*          Copyright Danila Kutenin, 2020-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "test_common.h"

using ::testing::Eq;

namespace miniselect {
namespace {

struct IndirectLess {
  // Non const comparator with deleted copy.
  template <class P>
  bool operator()(const P &x, const P &y) const {
    return *x < *y;
  }
  IndirectLess(const IndirectLess &) = default;
  IndirectLess &operator=(const IndirectLess &) = default;
  IndirectLess(IndirectLess &&) = default;
  IndirectLess &operator=(IndirectLess &&) = default;
};

struct CustomInt {
  size_t x = 0;
  bool operator<(const CustomInt& other) const {
    return x < other.x;
  }
};

template <typename Sorter>
class PartialSortTest : public ::testing::Test {
 public:
  static void TestSorts(size_t N, size_t M) {
    ASSERT_NE(N, 0);
    ASSERT_GE(N, M);
    SCOPED_TRACE(N);
    SCOPED_TRACE(M);
    std::vector<int> array(N);
    for (size_t i = 0; i < N; ++i) {
      array[i] = i;
    }
    auto array_smaller = array;
    std::mt19937_64 mersenne_engine;
    std::shuffle(array.begin(), array.end(), mersenne_engine);
    Sorter::Sort(array.begin(), array.begin() + M, array.end(),
                 std::greater<int>());
    for (size_t i = 0; i < M; ++i) {
      EXPECT_EQ(array[i], N - i - 1);
    }
    std::shuffle(array_smaller.begin(), array_smaller.end(), mersenne_engine);
    Sorter::Sort(array_smaller.begin(), array_smaller.begin() + M,
                 array_smaller.end());
    for (size_t i = 0; i < M; ++i) {
      EXPECT_EQ(array_smaller[i], i);
    }
  }

  static void TestRandomAccessIterator(size_t N, size_t M) {
    ASSERT_NE(N, 0);
    ASSERT_GT(N, M);
    SCOPED_TRACE(N);
    SCOPED_TRACE(M);
    std::vector<char> array(N);
    for (size_t i = 0; i < N; ++i) {
      array[i] = i;
    }
    std::mt19937_64 mersenne_engine;
    std::shuffle(array.begin(), array.end(), mersenne_engine);
    algorithms::IntegralCharIterator<char> beg(array.data());
    algorithms::IntegralCharIterator<char> mid(array.data() + M);
    algorithms::IntegralCharIterator<char> end(array.data() + array.size());
    Sorter::Sort(beg, mid, end, std::greater<char>());
    for (size_t i = 0; i < M; ++i) {
      EXPECT_EQ(array[i], N - i - 1);
    }
  }

  static void TestManyRandomAccessIterators(size_t N) {
    TestRandomAccessIterator(N, 0);
    TestRandomAccessIterator(N, 1);
    TestRandomAccessIterator(N, 2);
    TestRandomAccessIterator(N, N / 2 - 1);
    TestRandomAccessIterator(N, N / 2);
    TestRandomAccessIterator(N, N / 2 + 1);
    TestRandomAccessIterator(N, N - 2);
    TestRandomAccessIterator(N, N - 1);
  }

  static void TestRandomAccessIterators() {
    TestManyRandomAccessIterators(127);
  }

  static void TestSorts(size_t N) {
    TestSorts(N, 0);
    TestSorts(N, 1);
    TestSorts(N, 2);
    TestSorts(N, 3);
    TestSorts(N, N / 2 - 1);
    TestSorts(N, N / 2);
    TestSorts(N, N / 2 + 1);
    TestSorts(N, N - 2);
    TestSorts(N, N - 1);
    TestSorts(N, N);
  }

  static void TestManySorts() {
    TestSorts(10);
    TestSorts(256);
    TestSorts(257);
    TestSorts(499);
    TestSorts(500);
    TestSorts(997);
    TestSorts(1000);
    TestSorts(1000 * 100);
    TestSorts(1009);
    TestSorts(1009 * 109);
  }

  static void TestCustomComparators() {
    std::vector<std::unique_ptr<int>> v(1000);
    for (size_t i = 0; i < v.size(); ++i) {
      v[i] = std::make_unique<int>(i);
    }
    Sorter::Sort(v.begin(), v.begin() + v.size() / 2, v.end(), IndirectLess{});
    for (size_t i = 0; i < v.size() / 2; ++i) {
      ASSERT_NE(v[i], nullptr);
      EXPECT_EQ(*v[i], i);
    }
  }

  static void TestOnlyOperatorLess() {
    std::vector<CustomInt> v(1000);
    for (size_t i = 0; i < v.size(); ++i) {
      v[i].x = v.size() - i - 1;
    }
    Sorter::Sort(v.begin(), v.begin() + v.size() / 2, v.end());
    for (size_t i = 0; i < v.size() / 2; ++i) {
      EXPECT_EQ(v[i].x, i);
    }
  }
};

TYPED_TEST_SUITE(PartialSortTest, algorithms::All);

TYPED_TEST(PartialSortTest, TestSmall) {
  std::vector<std::string> v = {"ab", "aaa", "ab"};
  TypeParam::Sort(v.begin(), v.begin() + 1, v.end());
  EXPECT_THAT(v, Eq(std::vector<std::string>{"aaa", "ab", "ab"}));
  v = {"aba"};
  TypeParam::Sort(v.begin(), v.begin(), v.end());
  EXPECT_THAT(v, Eq(std::vector<std::string>{"aba"}));
  v.clear();
  TypeParam::Sort(v.begin(), v.end(), v.end());
  EXPECT_TRUE(v.empty());
}

TYPED_TEST(PartialSortTest, TestAnotherSmall) {
  std::vector<std::string> v = {"ab", "ab", "aaa"};
  TypeParam::Sort(v.begin(), v.begin() + 1, v.end());
  EXPECT_THAT(v, Eq(std::vector<std::string>{"aaa", "ab", "ab"}));
}

TYPED_TEST(PartialSortTest, TestEmptySmall) {
  std::vector<std::string> v = {"", ""};
  TypeParam::Sort(v.begin(), v.begin() + 1, v.end());
  EXPECT_THAT(v, Eq(std::vector<std::string>{"", ""}));
}

TYPED_TEST(PartialSortTest, TestBasic) { TestFixture::TestManySorts(); }

TYPED_TEST(PartialSortTest, TestComparators) {
  TestFixture::TestCustomComparators();
}

TYPED_TEST(PartialSortTest, TestRandomAccessIterators) {
  TestFixture::TestRandomAccessIterators();
}

TYPED_TEST(PartialSortTest, TestOnlyOperatorLess) {
  TestFixture::TestOnlyOperatorLess();
}

// The standard says that the order of other elements is unspecified even if
// nothing should be sorted so it fails for libcxx and PDQ which is Ok. Saving
// this test for a reference.
TYPED_TEST(PartialSortTest, DISABLED_TestEmpty) {
  std::vector<int> array(100);
  for (size_t i = 0; i < 100; ++i) {
    array[i] = i;
  }
  std::mt19937_64 mersenne_engine;
  std::shuffle(array.begin(), array.end(), mersenne_engine);
  size_t cmp = 0;
  auto copy_array = array;
  // Should be no effect.
  TypeParam::Sort(array.begin(), array.begin(), array.end(),
                  [&cmp](const auto &lhs, const auto &rhs) {
                    ++cmp;
                    return lhs < rhs;
                  });
  EXPECT_EQ(cmp, 0);
  EXPECT_EQ(copy_array, array);
}

}  // namespace
}  // namespace miniselect

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
