#pragma once
#include "src/Math/Factors.hpp"
namespace math_internal {
constexpr u64 rec(u64 a, u64 b, u64 m) {
 if (a == 0) return (b ^ 1) & 1;
 if (b == 0 || m == 1) return 1;
 u64 ret= 1, k= 1, tmp= 1, i= 0;
 for (const auto [p, e]: Factors(m)) {
  for (tmp= p - 1, i= e - (p == 2 && e > 3); --i;) tmp*= p;
  k= std::lcm(k, tmp);
 }
 auto mod= [m](u128 x) { return x < m ? x : x % m + m; };
 for (k= rec(a, b - 1, k), a= mod(a);; a= mod(u128(a) * a))
  if (k& 1 ? ret= mod(u128(ret) * a) : 0; !(k>>= 1)) return ret;
}
constexpr u64 mod_tetration(u64 a, u64 b, u64 m) { return (a= rec(a, b, m)) >= m ? a - m : a; }
}  // namespace math_internal
using math_internal::mod_tetration;