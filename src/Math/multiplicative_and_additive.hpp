#pragma once
#include <bits/stdc++.h>
/**
 * @title 乗法的関数・加法的関数
 * @category 数学
 * @see https://en.wikipedia.org/wiki/Arithmetic_function
 */

// BEGIN CUT HERE

namespace multiplicative_functions {
template <class T>
struct Totient {
  static constexpr T f(std::uint64_t p, short e) {
    T ret = p - 1;
    while (e-- > 1) ret *= p;
    return ret;
  }
  static std::vector<T> poly() { return {-1, 1}; }
};
template <class T>
struct Moebius {
  static constexpr T f(std::uint64_t, short e) { return (e == 0) - (e == 1); }
  static std::vector<T> poly() { return {-1}; }
};
template <class T>
struct Liouville {
  static constexpr T f(std::uint64_t, short e) { return e & 1 ? -1 : 1; }
  static std::vector<T> poly() { return {-1}; }
};
template <class T, std::uint64_t k>
struct Divisor {
  static constexpr T f(std::uint64_t p, short e) {
    T ret = 0, pk = 1, pkpw = 1, b = p;
    for (std::uint64_t kk = k; kk; kk >>= 1, b *= b)
      if (kk & 1) pk *= b;
    for (short i = 0; i <= e; i++, pkpw *= pk) ret += pkpw;
    return ret;
  }
  static std::vector<T> poly() {
    std::vector<T> ret(k + 1, 0);
    ret[0] += 1, ret[k] += 1;
    return ret;
  }
};
template <class T>
struct Dedekind {
  static constexpr T f(std::uint64_t p, short e) {
    T ret = p + 1;
    while (e-- > 1) ret *= p;
    return ret;
  }
  static std::vector<T> poly() { return {1, 1}; }
};
}  // namespace multiplicative_functions

namespace additive_functions {
// the total number of prime factors of n
template <class T>
struct BigOmega {
  static constexpr T f(std::uint64_t, short e) { return e; }
  static std::vector<T> poly() { return {1}; }
};
// the total number of different prime factors of n
template <class T>
struct LittleOmega {
  static constexpr T f(std::uint64_t, short) { return 1; }
  static std::vector<T> poly() { return {1}; }
};
// the sum of primes dividing n counting multiplicity
template <class T>
struct Sopfr {
  static constexpr T f(std::uint64_t p, short e) { return p * e; }
  static std::vector<T> poly() { return {0, 1}; }
};
// the sum of the distinct primes dividing n
template <class T>
struct Sopf {
  static constexpr T f(std::uint64_t p, short) { return p; }
  static std::vector<T> poly() { return {0, 1}; }
};
}  // namespace additive_functions
