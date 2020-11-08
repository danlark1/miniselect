[![License](https://img.shields.io/badge/License-Boost%201.0-lightblue.svg)](https://www.boost.org/LICENSE_1_0.txt)

miniselect : Various generic selection and partial ordering algorithms
======================================================================

`miniselect` is a C++ header-only library that contains various generic selection
and partial sorting algorithms with the ease of use, testing, advice on usage and
benchmarking.

Sorting is everywhere and there are many outstanding sorting algorithms that
compete in speed, comparison count and cache friendliness. However selection
algorithms are always a bit outside of the competition scope and this library
solves this problem with the following keys in mind:

* **Easy:** First-class, easy to use dependency and carefully documented APIs and algorithm properties.
* **Fast:** We do care about speed of the algorithms and provide reasonable implementations.
* **Standard compliant:** We provide C++11 compatible APIs that are compliant to the standard [`std::nth_element`](https://en.cppreference.com/w/cpp/algorithm/nth_element) and [`std::partial_sort`](https://en.cppreference.com/w/cpp/algorithm/partial_sort) functions including custom comparators and order guarantees. Just replace the names of the functions in your project and it should work!
* **Well tested:** We test all algorithms with a unified framework, under sanitizers and fuzzing.
* **Benchmarked:** We gather benchmarks for all implementations to better understand good and bad spots of the algorithms.

