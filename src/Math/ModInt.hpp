#pragma once
#include <bits/stdc++.h>
#include "src/Math/mod_inv.hpp"
#include "src/Math/ModIntPrototype.hpp"
namespace math_internal {
#define CE constexpr
struct m_b {};
struct s_b: m_b {};
template <class mod_t> CE bool is_modint_v= is_base_of_v<m_b, mod_t>;
template <class mod_t> CE bool is_staticmodint_v= is_base_of_v<s_b, mod_t>;
template <class MP, u64 MOD> struct SB: s_b {
protected:
 static CE MP md= MP(MOD);
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
  ret.x= op; \
  return ret; \
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
template <u64 MOD> using ModInt= conditional_t < (MOD < (1 << 30)) & MOD, MInt<int, u32, SB<MP_Mo<u32, u64, 32, 31>, MOD>>, conditional_t<(MOD < (1ull << 62)) & MOD, MInt<i64, u64, SB<MP_Mo<u64, u128, 64, 63>, MOD>>, conditional_t<MOD<INT_MAX, MInt<int, u32, SB<MP_Na, MOD>>, conditional_t<MOD <= UINT_MAX, MInt<i64, u32, SB<MP_Na, MOD>>, conditional_t<MOD <= (1ull << 41), MInt<i64, u64, SB<MP_Br2, MOD>>, MInt<i64, u64, SB<MP_D2B1, MOD>>>>>>>;
#undef CE
}
using math_internal::ModInt, math_internal::is_modint_v, math_internal::is_staticmodint_v;