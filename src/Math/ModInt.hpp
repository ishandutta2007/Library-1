#pragma once
#include <bits/stdc++.h>
#include "src/Math/mod_inv.hpp"
#include "src/Math/ModIntPrototype.hpp"
/**
 * @title ModInt
 * @category 数学
 */

// BEGIN CUT HERE
namespace math_internal {
struct modint_base {};
struct sta_mint_base : modint_base {};
struct run_mint_base : modint_base {};
template <class mod_t>
constexpr bool is_modint_v = is_base_of_v<modint_base, mod_t>;
template <class mod_t>
constexpr bool is_staticmodint_v = is_base_of_v<sta_mint_base, mod_t>;
template <class mod_t>
constexpr bool is_runtimemodint_v = is_base_of_v<run_mint_base, mod_t>;
template <class mod_pro_t, u64 MOD>
struct StaticB {
 protected:
  static constexpr mod_pro_t md = mod_pro_t(MOD);
};
template <class mod_pro_t, int id>
struct RuntimeB {
  static inline void set_mod(u64 m) { md = mod_pro_t(m); }

 protected:
  static inline mod_pro_t md;
};
template <class Int, class Uint, class B>
struct ModInt : public B {
  static constexpr inline auto modulo() { return B::md.modulo(); }
  constexpr ModInt() : x(0);
  constexpr ModInt(const ModInt &r) : x(r.x) {}
  template <class T, enable_if_t<is_modint_v<T>, nullptr_t> = nullptr>
  constexpr ModInt(T v) : ModInt(v.val()) {}
  template <class T,
            enable_if_t<is_convertible_v<T, __int128_t>, nullptr_t> = nullptr>
  constexpr ModInt(T n)
      : x(B::md.set(n < 0 ? modulo() - ((-n) % modulo()) : n)) {}
  constexpr ModInt operator-() const { return ModInt() - *this; }
#define FUNC(name, op)          \
  constexpr ModInt name const { \
    ModInt ret;                 \
    return ret.x = op, ret;     \
  }
  FUNC(operator+(const ModInt &r), B::md.plus(x, r.x))
  FUNC(operator-(const ModInt &r), B::md.diff(x, r.x))
  FUNC(operator*(const ModInt &r), B::md.mul(x, r.x))
  FUNC(pow(u64 k), math_internal::pow(x, k, B::md))
#undef FUNC
  constexpr ModInt operator/(const ModInt &r) const { return *this * r.inv(); }
  constexpr ModInt &operator+=(const ModInt &r) { return *this = *this + r; }
  constexpr ModInt &operator-=(const ModInt &r) { return *this = *this - r; }
  constexpr ModInt &operator*=(const ModInt &r) { return *this = *this * r; }
  constexpr ModInt &operator/=(const ModInt &r) { return *this = *this / r; }
  constexpr bool operator==(const ModInt &r) {
    return B::md.norm(x) == B::md.norm(r.x);
  }
  constexpr bool operator!=(const ModInt &r) { return !(*this == r); }
  constexpr bool operator<(const ModInt &r) {
    return B::md.norm(x) < B::md.norm(r.x);
  }
  constexpr inline ModInt inv() const { return mod_inv<Int>(val(), modulo()); }
  constexpr inline Uint val() const { return B::md.get(x); }
  friend ostream &operator<<(ostream &os, const ModInt &r) {
    return os << r.val();
  }
  friend istream &operator>>(istream &is, ModInt &r) {
    long long v;
    return is >> v, r = ModInt(v), is;
  }

 private:
  Uint x;
};
template <u64 MOD>
using StaticModInt =
    conditional_t <
    MOD<INT_MAX, ModInt<int, u32, StaticB<MIntPro_Na<u32>, MOD>>,
        conditional_t<MOD &(MOD < LLONG_MAX),
                      ModInt<long long, u64, StaticB<MIntPro_Montg, MOD>>,
                      ModInt<long long, u64, StaticB<MIntPro_Na<u64>, MOD>>>>;
class Montgomery {};
template <class Int, int id = -1>
using RuntimeModInt = conditional_t<
    is_same_v<Int, Montgomery>,
    ModInt<long long, u64, RuntimeB<MIntPro_Montg, id>>,
    conditional_t<disjunction_v<is_same<Int, long long>, is_same<Int, u64>>,
                  ModInt<long long, u64, RuntimeB<MIntPro_Na<u64>, id>>,
                  ModInt<int, u32, RuntimeB<MIntPro_Na<u32>, id>>>>;
}  // namespace math_internal
using math_internal::RuntimeModInt, math_internal::StaticModInt,
    math_internal::Montgomery, math_internal::is_runtimemodint_v,
    math_internal::is_modint_v, math_internal::is_staticmodint_v;