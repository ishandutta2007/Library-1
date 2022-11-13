#pragma once
#include <bits/stdc++.h>
#include "src/Math/mod_inv.hpp"
/**
 * @title ModInt
 * @category 数学
 */

// BEGIN CUT HERE
namespace modint_internal {
using namespace std;
struct modint_base {};
struct sta_mint_base : modint_base {};
struct run_mint_base : modint_base {};
template <class mod_t>
constexpr bool is_modint_v = is_base_of_v<modint_base, mod_t>;
template <class mod_t>
constexpr bool is_staticmodint_v = is_base_of_v<sta_mint_base, mod_t>;
template <class mod_t>
constexpr bool is_runtimemodint_v = is_base_of_v<run_mint_base, mod_t>;
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
  constexpr D inv() const {
    return mod_inv<typename D::Int>(((D *)this)->val(), D::mod);
  }
  constexpr D sqrt() const {
    if (((D *)this)->val() <= 1 || D::mod == 2) return *(D *)this;
    u64 e = (D::mod - 1) >> 1;
    D b = 0, d = -(*this), ret = 1, r2 = 0, b2 = 1;
    if (this->pow(e) != 1) return 0;  // no solution
    while (d.pow(e) == 1) d += b * 2 + 1, b += 1;
    auto mult = [d](D &u1, D &u2, D v1, D v2) {
      D tmp = u1 * v1 + u2 * v2 * d;
      u2 = u1 * v2 + u2 * v1, u1 = tmp;
    };
    for (++e;; mult(b, b2, b, b2)) {
      if (e & 1) mult(ret, r2, b, b2);
      if (!(e >>= 1)) return ret.val() <= D::mod / 2 ? ret : -ret;
    }
  }
  constexpr bool operator<(const D &r) const {
    return ((D *)this)->val() < r.val();
  }  // for set or map
  friend ostream &operator<<(ostream &os, const D &r) { return os << r.val(); }
  friend istream &operator>>(istream &is, D &r) {
    long long v;
    return is >> v, r = D(v), is;
  }
};
template <class B>
struct ModInt_Na : public B, public ModIntImpl<ModInt_Na<B>> {
  using Int = typename B::Int;
  using DUint = conditional_t<is_same_v<typename B::Uint, uint32_t>, u64, u128>;
  friend ModIntImpl<ModInt_Na<B>>;
  constexpr ModInt_Na() = default;
  template <class T, enable_if_t<is_modint_v<T>, nullptr_t> = nullptr>
  constexpr ModInt_Na(T n) : ModInt_Na(n.val()) {}
  template <class T,
            enable_if_t<is_convertible_v<T, __int128_t>, nullptr_t> = nullptr>
  constexpr ModInt_Na(T n) : x(n < 0 ? B::mod - ((-n) % B::mod) : n % B::mod) {}
#define ASSIGN(m, p) return x m## = B::mod & -((x p## = r.x) >= B::mod), *this
  constexpr ModInt_Na &operator+=(const ModInt_Na &r) { ASSIGN(-, +); }
  constexpr ModInt_Na &operator-=(const ModInt_Na &r) { ASSIGN(+, -); }
#undef ASSIGN
  constexpr ModInt_Na &operator*=(const ModInt_Na &r) {
    return x = (DUint)(x)*r.x % B::mod, *this;
  }
  constexpr bool operator==(const ModInt_Na &r) const { return x == r.x; }
  constexpr auto val() const { return x; }
  constexpr auto norm() const { return x; }

 private:
  typename B::Uint x = 0;
};
template <class B>
struct ModInt_Mon : public B, public ModIntImpl<ModInt_Mon<B>> {
  using Int = int64_t;
  using mod_t = ModInt_Mon;
  friend ModIntImpl<ModInt_Mon<B>>;
  constexpr ModInt_Mon() = default;
  template <class T, enable_if_t<is_modint_v<T>, nullptr_t> = nullptr>
  constexpr ModInt_Mon(T n) : ModInt_Mon(n.val()) {}
  template <class T,
            enable_if_t<is_convertible_v<T, __int128_t>, nullptr_t> = nullptr>
  constexpr ModInt_Mon(T n)
      : x(mul(n < 0 ? B::mod - ((-n) % B::mod) : n % B::mod, B::r2)) {}
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
  constexpr inline u64 norm() const { return x - (B::mod & -(x >= B::mod)); }

 private:
  static constexpr inline u64 reduce(const u128 &w) {
    return u64(w >> 64) + B::mod - ((u128(u64(w) * B::iv) * B::mod) >> 64);
  }
  static constexpr inline u64 mul(u64 l, u64 r) { return reduce(u128(l) * r); }
  u64 x = 0;
};
constexpr u64 mul_inv(u64 n, int e = 6, u64 x = 1) {
  return e ? mul_inv(n, e - 1, x * (2 - x * n)) : x;
}
template <u64 MOD>
struct StaticB_Na : sta_mint_base {
 protected:
  using Int = conditional_t < MOD < INT_MAX, int32_t,
        conditional_t<MOD<LLONG_MAX, int64_t, __int128_t>>;
  using Uint = conditional_t < MOD < INT_MAX, uint32_t,
        conditional_t<MOD<LLONG_MAX, u64, u128>>;
  static constexpr Uint mod = MOD;
};
template <u64 MOD>
struct StaticB_Mon : sta_mint_base {
 protected:
  static_assert(MOD & 1);
  static constexpr u64 mod = MOD, iv = mul_inv(mod), r2 = -u128(mod) % mod;
};
template <class I, int id = -1>
struct RuntimeB_Na : run_mint_base {
  static_assert(is_integral_v<I>);
  static inline void set_mod(I m) { mod = m; }

 protected:
  using Int = I;
  using Uint = make_unsigned_t<Int>;
  static inline Uint mod;
};
template <int id>
struct RuntimeB_Mon : run_mint_base {
  static inline void set_mod(u64 m) {
    assert(m & 1), iv = mul_inv(mod = m), r2 = -u128(m) % m;
  }

 protected:
  static inline u64 mod, iv, r2;
};
template <u64 mod>
using StaticModInt =
    conditional_t<mod &(INT_MAX <= mod) & (mod < LLONG_MAX),
                  ModInt_Mon<StaticB_Mon<mod>>, ModInt_Na<StaticB_Na<mod>>>;
struct Montgomery {};
template <class Int, int id = -1>
using RuntimeModInt =
    conditional_t<is_same_v<Int, Montgomery>, ModInt_Mon<RuntimeB_Mon<id>>,
                  ModInt_Na<RuntimeB_Na<Int, id>>>;
}  // namespace modint_internal
using modint_internal::RuntimeModInt, modint_internal::StaticModInt,
    modint_internal::Montgomery, modint_internal::is_runtimemodint_v,
    modint_internal::is_modint_v, modint_internal::is_staticmodint_v;
template <class mod_t, std::size_t LIM>
mod_t get_inv(int n) {
  static_assert(is_modint_v<mod_t>);
  static const auto m = mod_t::modulo();
  static mod_t dat[LIM];
  static int l = 1;
  if (l == 1) dat[l++] = 1;
  while (l <= n) dat[l++] = dat[m % l] * (m - m / l);
  return dat[n];
}
