#pragma once
#include <bits/stdc++.h>
#include "src/Math/mod_inv.hpp"
#include "src/Math/ModIntPrototype.hpp"
namespace math_internal {
#define CE constexpr
struct m_b {};
struct s_b: m_b {};
struct r_b: m_b {};
template <class mod_t> CE bool is_modint_v= is_base_of_v<m_b, mod_t>;
template <class mod_t> CE bool is_staticmodint_v= is_base_of_v<s_b, mod_t>;
template <class mod_t> CE bool is_runtimemodint_v= is_base_of_v<r_b, mod_t>;
template <class mpt, u64 MOD> struct SB: s_b {
protected:
 static CE mpt md= mpt(MOD);
};
template <class mpt, int id> struct RB: r_b {
 static inline void set_mod(u64 m) { md= mpt(m); }
protected:
 static inline mpt md;
};
template <class Int, class U, class B> struct MInt: public B {
 using Uint= U;
 static CE inline auto mod() { return B::md.mod; }
 CE MInt(): x(0) {}
 CE MInt(const MInt& r): x(r.x) {}
 template <class T, enable_if_t<is_modint_v<T>, nullptr_t> = nullptr> CE MInt(T v): x(B::md.set(v.val() % B::md.mod)) {}
 template <class T, enable_if_t<is_convertible_v<T, __int128_t>, nullptr_t> = nullptr> CE MInt(T n): x(B::md.set((n < 0 ? B::md.mod - (-n) % B::md.mod : n % B::md.mod))) {}
 CE MInt operator-() const { return MInt() - *this; }
#define FUNC(name, op) \
 CE MInt name const { \
  MInt ret; \
  return ret.x= op, ret; \
 }
 FUNC(operator+(const MInt& r), B::md.plus(x, r.x))
 FUNC(operator-(const MInt& r), B::md.diff(x, r.x))
 FUNC(operator*(const MInt& r), B::md.mul(x, r.x))
 FUNC(pow(u64 k), math_internal::pow(x, k, B::md))
#undef FUNC
 CE MInt operator/(const MInt& r) const { return *this * r.inv(); }
 CE MInt& operator+=(const MInt& r) { return *this= *this + r; }
 CE MInt& operator-=(const MInt& r) { return *this= *this - r; }
 CE MInt& operator*=(const MInt& r) { return *this= *this * r; }
 CE MInt& operator/=(const MInt& r) { return *this= *this / r; }
 CE bool operator==(const MInt& r) const { return B::md.norm(x) == B::md.norm(r.x); }
 CE bool operator!=(const MInt& r) const { return !(*this == r); }
 CE bool operator<(const MInt& r) const { return B::md.norm(x) < B::md.norm(r.x); }
 CE inline MInt inv() const { return mod_inv<Int>(val(), B::md.mod); }
 CE inline Uint val() const { return B::md.get(x); }
 friend ostream& operator<<(ostream& os, const MInt& r) { return os << r.val(); }
 friend istream& operator>>(istream& is, MInt& r) {
  i64 v;
  return is >> v, r= MInt(v), is;
 }
private:
 Uint x;
};
template <u64 MOD> using StaticModInt= conditional_t < MOD<INT_MAX, MInt<int, u32, SB<MP_Na<u32>, MOD>>, conditional_t<MOD&(MOD < LLONG_MAX), MInt<i64, u64, SB<MP_Mo, MOD>>, MInt<i64, u64, SB<MP_Na<u64>, MOD>>>>;
class Montgomery {};
template <class Int, int id= -1> using RuntimeModInt= conditional_t<is_same_v<Int, Montgomery>, MInt<i64, u64, RB<MP_Mo, id>>, conditional_t<disjunction_v<is_same<Int, i64>, is_same<Int, u64>>, MInt<i64, u64, RB<MP_Na<u64>, id>>, MInt<int, u32, RB<MP_Na<u32>, id>>>>;
#undef CE
}
using math_internal::RuntimeModInt, math_internal::StaticModInt, math_internal::Montgomery, math_internal::is_runtimemodint_v, math_internal::is_modint_v, math_internal::is_staticmodint_v;
template <class mod_t, size_t LIM> mod_t get_inv(int n) {
 static_assert(is_modint_v<mod_t>);
 static const auto m= mod_t::mod();
 static mod_t dat[LIM];
 static int l= 1;
 if (l == 1) dat[l++]= 1;
 while (l <= n) dat[l++]= dat[m % l] * (m - m / l);
 return dat[n];
}