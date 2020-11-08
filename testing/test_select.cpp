/*          Copyright Danila Kutenin, 2020-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "test_common.h"

namespace miniselect {
namespace {

struct IndirectLess {
  template <class P>
  bool operator()(const P &x, const P &y) const {
    return *x < *y;
  }
};

template <typename Selector>
class SelectTest : public ::testing::Test {
 public:
  using Base = Selector;

  static void TestSelects(size_t N, size_t M) {
    ASSERT_NE(N, 0);
    ASSERT_GT(N, M);
    SCOPED_TRACE(N);
    SCOPED_TRACE(M);
    std::vector<int> array(N);
    for (size_t i = 0; i < N; ++i) {
      array[i] = i;
    }
    auto array_smaller = array;
    std::mt19937_64 mersenne_engine;
    std::shuffle(array.begin(), array.end(), mersenne_engine);
    Selector::Select(array.begin(), array.begin() + M, array.end(),
                     std::greater<int>());
    EXPECT_EQ(array[M], N - M - 1);
    for (size_t i = 0; i < M; ++i) {
      EXPECT_GE(array[i], array[M]);
    }
    for (size_t i = M; i < N; ++i) {
      EXPECT_LE(array[i], array[M]);
    }
    std::shuffle(array_smaller.begin(), array_smaller.end(), mersenne_engine);
    Selector::Select(array_smaller.begin(), array_smaller.begin() + M,
                     array_smaller.end());
    EXPECT_EQ(array_smaller[M], M);
    for (size_t i = 0; i < M; ++i) {
      EXPECT_LE(array_smaller[i], array_smaller[M]);
    }
    for (size_t i = M; i < N; ++i) {
      EXPECT_GE(array_smaller[i], array_smaller[M]);
    }
  }

  static void TestSelects(size_t N) {
    TestSelects(N, 0);
    TestSelects(N, 1);
    TestSelects(N, 2);
    TestSelects(N, 3);
    TestSelects(N, N / 2 - 1);
    TestSelects(N, N / 2);
    TestSelects(N, N / 2 + 1);
    TestSelects(N, N - 2);
    TestSelects(N, N - 1);
  }

  static void TestManySelects() {
    TestSelects(10);
    TestSelects(256);
    TestSelects(257);
    TestSelects(499);
    TestSelects(500);
    TestSelects(997);
    TestSelects(1000);
    TestSelects(1000 * 100);
    TestSelects(1009);
    TestSelects(1009 * 109);
  }

  static void TestCustomComparators() {
    std::vector<std::unique_ptr<int>> v(1000);
    for (int i = 0; static_cast<std::size_t>(i) < v.size(); ++i) {
      v[i] = std::make_unique<int>(i);
    }
    Selector::Select(v.begin(), v.begin() + v.size() / 2, v.end(),
                     IndirectLess());
    EXPECT_EQ(*v[v.size() / 2], v.size() / 2);
    for (size_t i = 0; i < v.size() / 2; ++i) {
      ASSERT_NE(v[i], nullptr);
      EXPECT_LE(*v[i], v.size() / 2);
    }
    for (size_t i = v.size() / 2; i < v.size(); ++i) {
      ASSERT_NE(v[i], nullptr);
      EXPECT_GE(*v[i], v.size() / 2);
    }
  }

  static void TestRepeated(size_t N, size_t M) {

  }
};

TYPED_TEST_SUITE(SelectTest, algorithms::All);

TYPED_TEST(SelectTest, TestBasic) { TestFixture::TestManySelects(); }

TYPED_TEST(SelectTest, TestComparators) {
  TestFixture::TestCustomComparators();
}

TYPED_TEST(SelectTest, TestLast) {
  std::vector<int> array(100);
  for (size_t i = 0; i < 100; ++i) {
    array[i] = i;
  }
  auto array_smaller = array;
  std::mt19937_64 mersenne_engine;
  std::shuffle(array.begin(), array.end(), mersenne_engine);
  auto copy_array = array;
  // Should be no effect.
  size_t cmp = 0;
  TypeParam::Select(array.begin(), array.end(), array.end(),
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
