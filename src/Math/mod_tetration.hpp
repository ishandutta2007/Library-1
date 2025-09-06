#pragma once
#include <cstdint>
#include "src/NumberTheory/Factors.hpp"
namespace math_internal {
constexpr u64 mod_tetration(u64 a, u64 b, u64 m) {
 if (m == 1) return 0;
 if (a == 0) return (b ^ 1) & 1;
 if (b == 0) return 1;
 if (b == 1) return a % m;
 u64 l= 1;
 for (auto [p, e]: Factors(m))
  for (l*= (p - 1) / binary_gcd(l, p - 1), e-= (p == 2 && e > 3); --e;) l*= p;
 if (b= b == 2 ? a : mod_tetration(a, b - 1, l); !b) b= l;
 for (l= 1;; a= (u128(a) * a) % m)
  if (b& 1 ? l= (u128(l) * a) % m : 0; !(b>>= 1)) return l;
}
}
using math_internal::mod_tetration;