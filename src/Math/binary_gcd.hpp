#pragma once
#include <type_traits>
#include <algorithm>
template <class Int> int bsf(Int a) {
 if constexpr (std::disjunction_v<std::is_same<Int, __uint128_t>, std::is_same<Int, __int128_t>>) {
  uint64_t lo= a & uint64_t(-1);
  return lo ? __builtin_ctzll(lo) : 64 + __builtin_ctzll(a >> 64);
 } else if constexpr (std::disjunction_v<std::is_same<Int, uint64_t>, std::is_same<Int, int64_t>>) return __builtin_ctzll(a);
 else return __builtin_ctz(a);
}
template <class Int> Int binary_gcd(Int a, Int b) {
 if (a == 0 || b == 0) return a + b;
 int n= bsf(a), m= bsf(b), s;
 for (a>>= n, b>>= m; a != b;) {
  Int d= a - b;
  bool f= a > b;
  s= bsf(d), b= f ? b : a, a= (f ? d : -d) >> s;
 }
 return a << std::min(n, m);
}