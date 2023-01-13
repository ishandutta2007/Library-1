#pragma once
#include <bits/stdc++.h>
#include "src/Internal/Remainder.hpp"
namespace math_internal {
template <class Uint, class MP, u64... args> constexpr bool miller_rabin(Uint n) {
 const MP md(n);
 const Uint s= __builtin_ctzll(n - 1), d= n >> s, one= md.set(1), n1= md.norm(md.set(n - 1));
 for (auto a: {args...})
  if (Uint b= a % n; b)
   if (Uint p= md.norm(pow(md.set(b), d, md)); p != one)
    for (int i= s; p != n1; p= md.norm(md.mul(p, p)))
     if (!(--i)) return 0;
 return 1;
}
constexpr bool is_prime(u64 n) {
 if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
 if (n < (1 << 30)) return miller_rabin<u32, MP_Mo<u32, u64, 32, 31>, 2, 7, 61>(n);
 if (n < (1ull << 62)) return miller_rabin<u64, MP_Mo<u64, u128, 64, 63>, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
 return miller_rabin<u64, MP_D2B1, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
}
}
using math_internal::is_prime;