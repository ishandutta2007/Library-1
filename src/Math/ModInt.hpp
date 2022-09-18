#pragma once
#include <bits/stdc++.h>
/**
 * @title ModInt
 * @category 数学
 */

// BEGIN CUT HERE
namespace modint_internal {
using namespace std;
struct modint_base {};
struct sta_mint_base : modint_base {};
struct dyn_mint_base : modint_base {};
template <class mod_t>
constexpr bool is_modint_v = is_base_of_v<modint_base, mod_t>;
template <class mod_t>
constexpr bool is_staticmodint_v = is_base_of_v<sta_mint_base, mod_t>;
template <class mod_t>
constexpr bool is_dynamicmodint_v = is_base_of_v<dyn_mint_base, mod_t>;
using u64 = uint64_t;
using u128 = __uint128_t;
template <class D>
struct ModIntImpl {
  static constexpr inline auto modulo() { return D::mod; }
  constexpr D operator-() const { return D() -= (D &)*this; }
  constexpr D &operator/=(const D &r) { return (D &)*this *= r.inv(); }
  constexpr D operator+(const D &r) const { return D((D &)*this) += r; }
  constexpr D operator-(const D &r) const { return D((D &)*this) -= r; }
  constexpr D operator*(const D &r) const { return D((D &)*this) *= r; }
  constexpr D operator/(const D &r) const { return D((D &)*this) /= r; }
  constexpr bool operator!=(const D &r) const { return !((D &)*this == r); }
  constexpr D pow(u64 k) const {
    for (D ret(1), b((const D &)*this);; b *= b)
      if (k & 1 ? ret *= b : 0; !(k >>= 1)) return ret;
  }
  constexpr D inv() const { return pow(D::mod - 2); }
  friend ostream &operator<<(ostream &os, const D &r) { return os << r.val(); }
  friend istream &operator>>(istream &is, D &r) {
    long long v;
    return is >> v, r = D(v), is;
  }
};
template <class B>
struct ModInt_Na : public B, public ModIntImpl<ModInt_Na<B>> {
  using DUint = conditional_t<is_same_v<typename B::Uint, u64>, u128, u64>;
  friend ModIntImpl<ModInt_Na<B>>;
  constexpr ModInt_Na() = default;
  template <class T, enable_if_t<is_integral_v<T>, nullptr_t> = nullptr>
  constexpr ModInt_Na(T n) : x(n < 0 ? B::mod - ((-n) % B::mod) : n % B::mod) {}
  template <class T, enable_if_t<is_modint_v<T>, nullptr_t> = nullptr>
  constexpr ModInt_Na(T n) : ModInt_Na(n.val()) {}
#define ASSIGN(m, p) return x m## = B::mod & -((x p## = r.x) >= B::mod), *this
  constexpr ModInt_Na &operator+=(const ModInt_Na &r) { ASSIGN(-, +); }
  constexpr ModInt_Na &operator-=(const ModInt_Na &r) { ASSIGN(+, -); }
#undef ASSIGN
  constexpr ModInt_Na &operator*=(const ModInt_Na &r) {
    return x = (DUint)(x)*r.x % B::mod, *this;
  }
  constexpr bool operator==(const ModInt_Na &r) const { return x == r.x; }
  constexpr auto val() const { return x; }

 private:
  typename B::Uint x = 0;
};
template <class B>
struct ModInt_Mon : public B, public ModIntImpl<ModInt_Mon<B>> {
  using Uint = u64;
  using mod_t = ModInt_Mon;
  friend ModIntImpl<ModInt_Mon<B>>;
  constexpr ModInt_Mon() = default;
  template <class T, enable_if_t<is_integral_v<T>, nullptr_t> = nullptr>
  constexpr ModInt_Mon(T n)
      : x(mul(n < 0 ? B::mod - ((-n) % B::mod) : n % B::mod, B::r2)) {}
  template <class T, enable_if_t<is_modint_v<T>, nullptr_t> = nullptr>
  constexpr ModInt_Mon(T n) : ModInt_Mon(n.val()) {}
#define ASGN(op, a) return x op## = a, x += (B::mod << 1) & -(x >> 63), *this
  constexpr mod_t &operator+=(const mod_t &r) { ASGN(+, r.x - (B::mod << 1)); }
  constexpr mod_t &operator-=(const mod_t &r) { ASGN(-, r.x); }
#undef ASGN
  constexpr mod_t &operator*=(const mod_t &r) { return x = mul(x, r.x), *this; }
  constexpr bool operator==(const mod_t &r) const { return norm() == r.norm(); }
  constexpr u64 val() const {
    u64 ret = reduce(x) - B::mod;
    return ret + (B::mod & -(ret >> 63));
  }

 private:
  static constexpr inline u64 reduce(const u128 &w) {
    return u64(w >> 64) + B::mod - ((u128(u64(w) * B::iv) * B::mod) >> 64);
  }
  static constexpr inline u64 mul(u64 l, u64 r) { return reduce(u128(l) * r); }
  u64 x = 0;
  constexpr inline u64 norm() const { return x - (B::mod & -(x >= B::mod)); }
};
constexpr u64 mul_inv(u64 n, int e = 6, u64 x = 1) {
  return e ? mul_inv(n, e - 1, x * (2 - x * n)) : x;
}
template <u64 MOD>
struct StaticB_Na : sta_mint_base {
 protected:
  using Uint = conditional_t < MOD<UINT_MAX, uint32_t, u64>;
  static constexpr Uint mod = MOD;
};
template <u64 MOD>
struct StaticB_Mon : sta_mint_base {
 protected:
  static_assert(MOD & 1);
  static constexpr u64 mod = MOD, iv = mul_inv(mod), r2 = -u128(mod) % mod;
};
template <class Int, int id = -1>
struct DynamicB_Na : dyn_mint_base {
  static_assert(is_integral_v<Int>);
  static inline void set_mod(Int m) { mod = m; }

 protected:
  using Uint = make_unsigned_t<Int>;
  static inline Uint mod;
};
template <int id>
struct DynamicB_Mon : dyn_mint_base {
  static inline void set_mod(u64 m) {
    assert(m & 1), iv = mul_inv(mod = m), r2 = -u128(m) % m;
  }

 protected:
  static inline u64 mod, iv, r2;
};
template <u64 mod>
using StaticModInt =
    conditional_t<mod &(mod >= UINT_MAX), ModInt_Mon<StaticB_Mon<mod>>,
                  ModInt_Na<StaticB_Na<mod>>>;
struct Montgomery {};
template <class Int, int id = -1>
using DynamicModInt =
    conditional_t<is_same_v<Int, Montgomery>, ModInt_Mon<DynamicB_Mon<id>>,
                  ModInt_Na<DynamicB_Na<Int, id>>>;
}  // namespace modint_internal
using modint_internal::DynamicModInt, modint_internal::StaticModInt,
    modint_internal::Montgomery, modint_internal::is_dynamicmodint_v,
    modint_internal::is_modint_v, modint_internal::is_staticmodint_v;