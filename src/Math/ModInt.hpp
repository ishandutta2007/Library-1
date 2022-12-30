#pragma once
#include <bits/stdc++.h>
#include "src/Math/mod_inv.hpp"
#include "src/Math/ModIntPrototype.hpp"
namespace math_internal {
struct m_b {};
struct s_b: m_b {};
struct r_b: m_b {};
template <class mod_t> constexpr bool is_modint_v= is_base_of_v<m_b, mod_t>;
template <class mod_t> constexpr bool is_staticmodint_v= is_base_of_v<s_b, mod_t>;
template <class mod_t> constexpr bool is_runtimemodint_v= is_base_of_v<r_b, mod_t>;
template <class mpt, u64 MOD> struct SB: s_b {
 protected:
  static constexpr mpt md= mpt(MOD);
};
template <class mpt, int id> struct RB: r_b {
  static inline void set_mod(u64 m) { md= mpt(m); }
 protected:
  static inline mpt md;
};
template <class Int, class U, class B> struct MInt: public B {
  using Uint= U;
  static constexpr inline auto modulo() { return B::md.mod; }
  constexpr MInt(): x(0) {}
  constexpr MInt(const MInt &r): x(r.x) {}
  template <class T, enable_if_t<is_modint_v<T>, nullptr_t> = nullptr> constexpr MInt(T v): x(B::md.set(v.val() % B::md.mod)) {}
  template <class T, enable_if_t<is_convertible_v<T, __int128_t>, nullptr_t> = nullptr> constexpr MInt(T n): x(B::md.set((n < 0 ? B::md.mod - (-n) % B::md.mod : n % B::md.mod))) {}
  constexpr MInt operator-() const { return MInt() - *this; }
#define FUNC(name, op) \
  constexpr MInt name const { \
    MInt ret; \
    return ret.x= op, ret; \
  }
  FUNC(operator+(const MInt &r), B::md.plus(x, r.x))
  FUNC(operator-(const MInt &r), B::md.diff(x, r.x))
  FUNC(operator*(const MInt &r), B::md.mul(x, r.x))
  FUNC(pow(u64 k), math_internal::pow(x, k, B::md))
#undef FUNC
  constexpr MInt operator/(const MInt &r) const { return *this * r.inv(); }
  constexpr MInt &operator+=(const MInt &r) { return *this= *this + r; }
  constexpr MInt &operator-=(const MInt &r) { return *this= *this - r; }
  constexpr MInt &operator*=(const MInt &r) { return *this= *this * r; }
  constexpr MInt &operator/=(const MInt &r) { return *this= *this / r; }
  constexpr bool operator==(const MInt &r) const { return B::md.norm(x) == B::md.norm(r.x); }
  constexpr bool operator!=(const MInt &r) const { return !(*this == r); }
  constexpr bool operator<(const MInt &r) const { return B::md.norm(x) < B::md.norm(r.x); }
  constexpr inline MInt inv() const { return mod_inv<Int>(val(), B::md.mod); }
  constexpr inline Uint val() const { return B::md.get(x); }
  friend ostream &operator<<(ostream &os, const MInt &r) { return os << r.val(); }
  friend istream &operator>>(istream &is, MInt &r) {
    long long v;
    return is >> v, r= MInt(v), is;
  }
 private:
  Uint x;
};
template <u64 MOD> using StaticModInt= conditional_t < MOD<INT_MAX, MInt<int, u32, SB<MP_Na<u32>, MOD>>, conditional_t<MOD &(MOD < LLONG_MAX), MInt<long long, u64, SB<MP_Mo, MOD>>, MInt<long long, u64, SB<MP_Na<u64>, MOD>>>>;
class Moomery {};
template <class Int, int id= -1> using RuntimeModInt= conditional_t<is_same_v<Int, Moomery>, MInt<long long, u64, RB<MP_Mo, id>>, conditional_t<disjunction_v<is_same<Int, long long>, is_same<Int, u64>>, MInt<long long, u64, RB<MP_Na<u64>, id>>, MInt<int, u32, RB<MP_Na<u32>, id>>>>;
}
using math_internal::RuntimeModInt, math_internal::StaticModInt, math_internal::Moomery, math_internal::is_runtimemodint_v, math_internal::is_modint_v, math_internal::is_staticmodint_v;
template <class mod_t, std::size_t LIM> mod_t get_inv(int n) {
  static_assert(is_modint_v<mod_t>);
  static const auto m= mod_t::modulo();
  static mod_t dat[LIM];
  static int l= 1;
  if (l == 1) dat[l++]= 1;
  while (l <= n) dat[l++]= dat[m % l] * (m - m / l);
  return dat[n];
}