#pragma once
#include <type_traits>
#include <algorithm>
template <class Int> int bsf(Int a) {
 if constexpr (sizeof(Int) == 16) {
  uint64_t lo= a & uint64_t(-1);
  return lo ? __builtin_ctzll(lo) : 64 + __builtin_ctzll(a >> 64);
 } else if constexpr (sizeof(Int) == 8) return __builtin_ctzll(a);
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