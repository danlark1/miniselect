[![License](https://img.shields.io/badge/License-Boost%201.0-lightblue.svg)](https://www.boost.org/LICENSE_1_0.txt)

miniselect : Generic selection and partial ordering algorithms
==============================================================

`miniselect` is a C++ header-only library that contains various generic selection
and partial sorting algorithms with the ease of use, testing, advice on usage and
benchmarking.

Sorting is everywhere and there are many outstanding sorting algorithms that
compete in speed, comparison count and cache friendliness. However selection
algorithms are always a bit outside of the competition scope and this library
solves this problem with Modern C++.

* **Easy:** First-class, easy to use dependency and carefully documented APIs and algorithm properties.
* **Fast:** We do care about speed of the algorithms and provide reasonable implementations.
* **Standard compliant:** We provide C++11 compatible APIs that are compliant to the standard [`std::nth_element`](https://en.cppreference.com/w/cpp/algorithm/nth_element) and [`std::partial_sort`](https://en.cppreference.com/w/cpp/algorithm/partial_sort) functions including custom comparators and order guarantees. Just replace the names of the functions in your project and it should work!
* **Well tested:** We test all algorithms with a unified framework, under sanitizers and fuzzing.
* **Benchmarked:** We gather benchmarks for all implementations to better understand good and bad spots.

Table of Contents
-----------------

* [Quick Start](#quick-start)
* [Testing](#testing)
* [Documentation](#documentation)
* [Performance results](#performance-results)
* [Real-world usage](#real-world-usage)
* [Contributing](#contributing)
* [Motivation](#motivation)
* [License](#license)

Quick Start
-----------

You can either include this project as a cmake dependency and then use the
headers that are provided in the [include](./include) folder or just pass the
[include](./include) folder to your compiler.

```cpp
#include <iostream>
#include <vector>

#include "miniselect/median_of_ninthers.h"

int main() {
  std::vector<int> v = {1, 8, 4, 3, 2, 9, 0, 7, 6, 5};
  miniselect::median_of_ninthers_select(v.begin(), v.begin() + 5, v.end());
  for (const int i : v) {
    std::cout << i << ' ';
  }
  return 0;
}
// Compile it `clang++/g++ -I$DIRECTORY/miniselect/include/ example.cpp -std=c++11 -O3 -o example
// Possible output: 0 1 4 3 2 5 8 7 6 9
```

Examples can be found in [examples](./examples).

We support all compilers starting from GCC 7 and Clang 6. We are also planning
to support Windows, for now it is best effort but no issues are known so far.

More on which algorithms are available, see [documentation](#documentation).

Testing
-------

To test and benchmark, we use [Google benchmark](https://github.com/google/benchmark) library.
Simply do in the root directory:

```console
# Check out the library.
$ git clone https://github.com/google/benchmark.git
# Benchmark requires Google Test as a dependency. Add the source tree as a subdirectory.
$ git clone https://github.com/google/googletest.git benchmark/googletest
$ mkdir build && cd build
$ cmake -DMINISELECT_TESTING=on ..
$ make -j
```

It will create two tests and two benchmarks `test_sort`, `test_select`,
`benchmark_sort`, `benchmark_select`. Use them to validate or contribute.

Documentation
-------------

There are several selection algorithms available, further ![\large n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+n) is the number
of elements in the array, ![\large k](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+k) is the selection element that is needed to be found:

| Name                                  | Average                                                                                                   | Best Case                                                                                                 | Worst Case                                                                                                                | Stable    | Deterministic     | Comparisons                                                                                                                                                                                                                                                                                                                               |
|------------------------------------   |---------------------------------------------------------------------------------------------------------  |---------------------------------------------------------------------------------------------------------  |-----------------------------------------------------------------------------------------------------------------------    |--------   |---------------    |----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   |
| pdqselect                             | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n\log n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%5Clog+n%29)     |   No      |      Yes          | At least ![\large 2n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2n). Random data around ![\large 2.5n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2.5n)                                                                                                          |
| FloydRivest                           | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n^2 )](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%5E2+%29)           |   No      |      Yes          | On average ![\large n + \min(k, n - k) + O(\sqrt{n \log n})](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+n+%2B+%5Cmin%28k%2C+n+-+k%29+%2B+O%28%5Csqrt%7Bn+%5Clog+n%7D%29)                                                                                                                              |
| MedianOfMedians                       | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)                   |   No      |      Yes          | Between ![\large 2n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2n) and ![\large 22n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+22n). Random data around  ![\large 2.5n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2.5n)    |
| MedianOfNinthers                      | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)                   |   No      |      Yes          | Between ![\large 2n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2n) and ![\large 12n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+12.5n). Random data around ![\large 2n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2n)       |
| NthElement libstdc++ (introselect)    | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n\log n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%5Clog+n%29)     |   No      |      Yes          | At least ![\large 2n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2n). Random data around ![\large 3n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+3n)                                                                                                              |
| NthElement libc++ (median of 3)       | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29)   | ![\large O(n^2 )](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%5E2+%29)           |   No      |      Yes          | At least ![\large 2n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+2n). Random data around ![\large 3n](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+3n)                                                                                                              |

For sorting the situation is similar except every line adds ![\large O(k\log k)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28k%5Clog+k%29) comparisons with one exception called partial sorting in C++ standard library.

| Name              | Average Case                                                                                                          | Best Case                                                                                               | Worst Case                                                                                                            | Stable | Deterministic | Comparisons                                                                                                                                                    |
|-------------------|-----------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|--------|---------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------|
| std::partial_sort | ![\large O(n\log k)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%5Clog+k%29) | ![\large O(n)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%29) | ![\large O(n\log k)](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+O%28n%5Clog+k%29) | No     | Yes           | ![\large n\log k](https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle+%5Clarge+n%5Clog+k) on average, for some data patterns might be better |


Performance results
-------------------

Real-world usage
----------------

Contributing
------------

Motivation
----------

License
-------

The code is made available under the [Boost License 1.0](https://boost.org/LICENSE_1_0.txt).

Third-Party Libraries Used and Adjusted
---------------------------------------

| Library             | License                                                                                          |
|---------------------|--------------------------------------------------------------------------------------------------|
| pdqsort             | [MIT](https://github.com/orlp/pdqsort/blob/47a46767d76fc852284eaa083e4b7034ee6e2559/license.txt) |
| MedianOfNinthers    | [Boost License 1.0](https://github.com/andralex/MedianOfNinthers/blob/master/LICENSE_1_0.txt)    |

