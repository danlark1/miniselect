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
static void BM_sel(benchmark::State& state) {
  auto vec = DataGen::Gen(kSize);
  const size_t arg = state.range(0);
  size_t cnt = 0;
  size_t access_count = 0;
  size_t comparison_count = 0;
  miniselect::datagens::CountingIterator begin_v(&vec, 0, &access_count);
  miniselect::datagens::CountingIterator end_v(&vec, vec.size(), &access_count);
  for (auto _ : state) {
    Impl::Select(begin_v, begin_v + arg, end_v,
                 [&comparison_count](const auto& left, const auto& right) {
                   comparison_count++;
                   return left < right;
                 });
    ++cnt;
    benchmark::DoNotOptimize(vec[arg]);
  }
  state.counters["Array Accesses"] = 1.0 * access_count / cnt;
  state.counters["Comparisons"] = 1.0 * comparison_count / cnt;
}

BENCH(BM_sel);

}  // namespace
}  // namespace miniselect

BENCHMARK_MAIN();
