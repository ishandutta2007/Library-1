#pragma once
#include <bits/stdc++.h>
template <class Int> constexpr inline Int mod_inv(Int a, Int mod) {
  static_assert(std::is_signed_v<Int>);
  Int x= 1, y= 0, b= mod;
  for (Int q= 0, z= 0, c= 0; b;)
    z= x, c= a, x= y, y= z - y * (q= a / b), a= b, b= c - b * q;
  return assert(a == 1), x < 0 ? mod - (-x) % mod : x % mod;
}