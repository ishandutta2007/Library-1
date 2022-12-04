#pragma once
#include <bits/stdc++.h>
/**
 * @title ModIntのプロトタイプ
 * @category 数学
 * モンゴメリとか
 */

// BEGIN CUT HERE
namespace math_internal {
using namespace std;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;
struct MIntPro_Montg {
  const u64 mod;
  constexpr MIntPro_Montg() : mod(0), iv(0), r2(0) {}
  constexpr MIntPro_Montg(u64 m) : mod(m), iv(inv(m)), r2(-u128(mod) % mod) {}
  constexpr inline u64 mul(u64 l, u64 r) const { return reduce(u128(l) * r); }
#define BOP(op, a) return l op## = a, l += (mod << 1) & -(l >> 63)
  constexpr inline u64 plus(u64 l, u64 r) const { BOP(+, r - (mod << 1)); }
  constexpr inline u64 diff(u64 l, u64 r) const { BOP(-, r); }
#undef BOP
  constexpr inline u64 set(u64 n) const { return mul(n, r2); }
  constexpr inline u64 get(u64 n) const {
    u64 ret = reduce(n) - mod;
    return ret + (mod & -(ret >> 63));
  }
  constexpr inline u64 norm(u64 n) const { return n - (mod & -(n >= mod)); }

 private:
  const u64 iv, r2;
  constexpr u64 inv(u64 n, int e = 6, u64 x = 1) {
    return e ? inv(n, e - 1, x * (2 - x * n)) : x;
  }
  constexpr inline u64 reduce(const u128 &w) const {
    return u64(w >> 64) + mod - ((u128(u64(w) * iv) * mod) >> 64);
  }
};
template <class Uint>
class MIntPro_Na {
  using DUint = conditional_t<is_same_v<Uint, u32>, u64, u128>;

 public:
  const Uint mod;
  constexpr MIntPro_Na() : mod(0){};
  constexpr MIntPro_Na(Uint m) : mod(m) {}
  constexpr inline Uint mul(Uint l, Uint r) const { return DUint(l) * r % mod; }
#define BOP(m, p) return l m## = mod & -((l p## = r) >= mod)
  constexpr inline Uint plus(Uint l, Uint r) const { BOP(-, +); }
  constexpr inline Uint diff(Uint l, Uint r) const { BOP(+, -); }
#undef BOP
  static constexpr inline Uint set(Uint n) { return n; }
  static constexpr inline Uint get(Uint n) { return n; }
  static constexpr inline Uint norm(Uint n) { return n; }
};
template <class Uint, class mod_pro_t>
constexpr Uint pow(Uint x, u64 k, const mod_pro_t &md) {
  for (Uint ret = md.set(1);; x = md.mul(x, x))
    if (k & 1 ? ret = md.mul(ret, x) : 0; !(k >>= 1)) return ret;
}
}  // namespace math_internal