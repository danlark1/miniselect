#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "test_common.h"

template <class Iter, class... T>
void ChooseImplementation(uint8_t byte, Iter beg, Iter end, Iter partition_iter,
                          const ::testing::Types<T...>&) {
  static_assert(sizeof...(T) < 256);
  int i = 0;
  constexpr size_t size = sizeof...(T);
  (
      [&]() {
        if (byte % size == i++) {
          T::Select(beg, partition_iter, end);
        }
      }(),
      ...);
}

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data,
                                      std::size_t size) {
  if (size <= 3) return 0;
  uint8_t impl = data[0];
  uint8_t partition_point = data[1];
  size = std::min(std::size_t{129}, size) - 2;
  partition_point %= size;
  std::vector<char> working(data + 2, data + size + 2);
  miniselect::algorithms::IntegralCharIterator<char> beg(working.data());
  miniselect::algorithms::IntegralCharIterator<char> ending(working.data() +
                                                            working.size());
  auto canonical = working;
  const auto partition_iter = beg + partition_point;
  ChooseImplementation(impl, beg, ending, partition_iter,
                       miniselect::algorithms::All{});
  if (partition_iter != ending) {
    const auto& nth = *partition_iter;
    bool is_error = false;
    if (!std::all_of(beg, partition_iter,
                     [&](const auto& v) { return v <= nth; })) {
      is_error = true;
    }
    if (!std::all_of(partition_iter, ending,
                     [&](const auto& v) { return v >= nth; })) {
      is_error = true;
    }
    if (is_error) {
    fail:
      std::cerr << "FAILED!\nCanonical: ";
      for (const auto& s : canonical) {
        std::cerr << static_cast<int>(s) << ' ';
      }
      std::cerr << std::endl;
      std::cerr << "Got: ";
      for (const auto& s : working) {
        std::cerr << static_cast<int>(s) << ' ';
      }
      std::cerr << std::endl;
      std::cerr << "partition_iter = " << static_cast<int>(partition_iter - beg)
                << std::endl;
      std::abort();
    }
  }

  return 0;
}
