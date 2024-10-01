#pragma once
#include <utility>
#include <type_traits>
#include <cassert>
template <class Uint> constexpr inline Uint mod_inv(Uint a, Uint mod) {
 std::make_signed_t<Uint> x= 1, y= 0;
 for (Uint q= 0, b= mod; b;) x= std::exchange(y, x - y * (q= a / b)), a= std::exchange(b, a - b * q);
 return assert(a == 1), x < 0 ? mod - (-x) % mod : x % mod;
}