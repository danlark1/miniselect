/*          Copyright Danila Kutenin, 2020-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
#include <benchmark/benchmark.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>

#include "bench_common.h"
#include "test_common.h"

namespace miniselect {
namespace {

static constexpr size_t kSize = 65536;

template <class DataGen, class Impl>
static void BM_sort(benchmark::State& state) {
  auto vec = DataGen::Gen(kSize);
  const size_t arg = state.range(0);
  size_t cnt = 0;
  size_t cmp = 0;
  for (auto _ : state) {
    Impl::Sort(vec.begin(), vec.begin() + arg, vec.end(),
               [&cmp](const auto& left, const auto& right) {
                 cmp++;
                 return left < right;
               });
    ++cnt;
    benchmark::DoNotOptimize(vec[arg]);
  }
  state.counters["Comparisons"] = 1.0 * cmp / cnt;
}

template <class DataGen, class Impl>
static void BM_sort_accesses(benchmark::State& state) {
  auto vec = DataGen::Gen(kSize);
  const size_t arg = state.range(0);
  size_t cnt = 0;
  size_t cmp = 0;
  miniselect::datagens::CountingIterator begin_v(&vec, 0, &cmp);
  miniselect::datagens::CountingIterator end_v(&vec, vec.size(), &cmp);
  for (auto _ : state) {
    Impl::Sort(begin_v, begin_v + arg, end_v);
    ++cnt;
    benchmark::DoNotOptimize(vec[arg]);
  }
  state.counters["Array Accesses"] = 1.0 * cmp / cnt;
}

BENCH(BM_sort);
BENCH(BM_sort_accesses);

}  // namespace
}  // namespace miniselect

BENCHMARK_MAIN();
