#pragma once
#include <bits/stdc++.h>
#include "src/Math/Factors.hpp"
template <std::uint64_t MOD> class ModIntExp {
 static_assert(MOD < 1uLL << 63, "MOD must be smaller than 2^63");
 using Uint= std::conditional_t < MOD<UINT_MAX, std::uint32_t, std::uint64_t>;
 using DUint= std::conditional_t<std::is_same_v<Uint, std::uint64_t>, __uint128_t, std::uint64_t>;
 using mod_t= ModIntExp;
 static constexpr inline Uint mod(DUint x) { return x < MOD * 2 ? Uint(x) : Uint(x % MOD) + MOD; }
 static constexpr inline Uint mul(Uint a, Uint b) { return mod(DUint(a) * b); }
 static constexpr inline Uint pow(Uint b, Uint k) {
  for (Uint ret(1);; b= mul(b, b))
   if (k & 1 ? ret= mul(ret, b) : 0; !(k>>= 1)) return ret;
 }
 static constexpr inline std::uint64_t f(std::uint64_t x) {
  std::uint64_t ret= 1, i= 0, tmp= 1;
  for (const auto &[p, e]: Factors(x)) {
   for (tmp= p - 1, i= e - (p == 2 && e > 3); --i;) tmp*= p;
   ret= std::lcm(ret, tmp);
  }
  return ret;
 }
public:
 Uint a;
 ModIntExp<f(MOD)> b;
 constexpr ModIntExp()= default;
 constexpr ModIntExp(std::uint64_t x): a(mod(x)), b(x) {}
 constexpr ModIntExp(Uint a_, ModIntExp<f(MOD)> b_): a(a_), b(b_) {}
 constexpr Uint val() const { return a < MOD ? a : a - MOD; }
 constexpr mod_t &operator*=(const mod_t &r) { return a= mul(a, r.a), b*= r.b, *this; }
 constexpr mod_t &operator+=(const mod_t &r) { return a-= MOD & -((a+= r.a) >= MOD * 2), b+= r.b, *this; }
 constexpr mod_t operator*(const mod_t &r) const { return mod_t(*this)*= r; }
 constexpr mod_t operator+(const mod_t &r) const { return mod_t(*this)+= r; }
 constexpr mod_t pow(const mod_t &r) const { return mod_t{pow(a, r.b.a), b.pow(r.b)}; };
};
template <> struct ModIntExp<1> {
 using mod_t= ModIntExp;
 bool a;
 constexpr ModIntExp(): a(0) {}
 constexpr ModIntExp(std::uint64_t x): a(x) {}
 constexpr std::uint32_t val() { return 0; }
 constexpr mod_t &operator*=(const mod_t &r) { return a&= r.a, *this; }
 constexpr mod_t &operator+=(const mod_t &r) { return a|= r.a, *this; }
 constexpr mod_t operator*(const mod_t &r) const { return mod_t(*this)*= r; }
 constexpr mod_t operator+(const mod_t &r) const { return mod_t(*this)+= r; }
 constexpr mod_t pow(const mod_t &r) const { return {a || !r.a}; };
};
template <std::uint64_t MOD> std::ostream &operator<<(std::ostream &os, const ModIntExp<MOD> &r) { return os << r.val(); }
template <std::uint64_t MOD> std::istream &operator>>(std::istream &is, ModIntExp<MOD> &r) {
 std::uint64_t v;
 return is >> v, r= ModIntExp<MOD>(v), is;
}
