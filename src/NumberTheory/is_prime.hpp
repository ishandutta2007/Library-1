#pragma once
#include "src/Internal/Remainder.hpp"
namespace math_internal {
template <class Uint, class MP, u32... args> constexpr bool miller_rabin(Uint n) {
 const MP md(n);
 const Uint s= __builtin_ctzll(n - 1), d= n >> s, one= md.set(1), n1= md.norm(md.set(n - 1));
 for (u32 a: (u32[]){args...})
  if (Uint b= a % n; b)
   if (Uint p= md.norm(pow(md.set(b), d, md)); p != one)
    for (int i= s; p != n1; p= md.norm(md.mul(p, p)))
     if (!(--i)) return 0;
 return 1;
}
}
constexpr bool is_prime(unsigned long long n) {
 if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
 if (n < (1 << 30)) return math_internal::miller_rabin<unsigned, math_internal::MP_Mo, 2, 7, 61>(n);
 if (n < (1ull << 62)) return math_internal::miller_rabin<unsigned long long, math_internal::MP_Mo, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
 if (n < (1ull << 63)) return math_internal::miller_rabin<unsigned long long, math_internal::MP_D2B1_1, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
 return math_internal::miller_rabin<unsigned long long, math_internal::MP_D2B1_2, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
}