#pragma once
#include "src/Math/Factors.hpp"
template <uint64_t MOD> class ModInt_Exp {
 static_assert(MOD < 1uLL << 63, "MOD must be smaller than 2^63");
 using Uint= std::conditional_t < MOD<(1ull << 32), uint32_t, uint64_t>;
 using DUint= std::conditional_t<std::is_same_v<Uint, uint64_t>, __uint128_t, uint64_t>;
 using mod_t= ModInt_Exp;
 static constexpr inline Uint mod(DUint x) { return x < MOD * 2 ? Uint(x) : Uint(x % MOD) + MOD; }
 static constexpr inline Uint mul(Uint a, Uint b) { return mod(DUint(a) * b); }
 static constexpr inline Uint pow(Uint b, Uint k) {
  for (Uint ret(1);; b= mul(b, b))
   if (k & 1 ? ret= mul(ret, b) : 0; !(k>>= 1)) return ret;
 }
 static constexpr inline uint64_t f(uint64_t x) {
  uint64_t ret= 1, i= 0, tmp= 1;
  for (const auto &[p, e]: Factors(x)) {
   for (tmp= p - 1, i= e - (p == 2 && e > 3); --i;) tmp*= p;
   ret*= tmp / binary_gcd(ret, tmp);
  }
  return ret;
 }
public:
 Uint a;
 ModInt_Exp<f(MOD)> b;
 constexpr ModInt_Exp()= default;
 constexpr ModInt_Exp(uint64_t x): a(mod(x)), b(x) {}
 constexpr ModInt_Exp(Uint a_, ModInt_Exp<f(MOD)> b_): a(a_), b(b_) {}
 constexpr Uint val() const { return a < MOD ? a : a - MOD; }
 constexpr mod_t &operator*=(const mod_t &r) { return a= mul(a, r.a), b*= r.b, *this; }
 constexpr mod_t &operator+=(const mod_t &r) { return a-= MOD & -((a+= r.a) >= MOD * 2), b+= r.b, *this; }
 constexpr mod_t operator*(const mod_t &r) const { return mod_t(*this)*= r; }
 constexpr mod_t operator+(const mod_t &r) const { return mod_t(*this)+= r; }
 constexpr mod_t pow(const mod_t &r) const { return mod_t{pow(a, r.b.a), b.pow(r.b)}; };
};
template <> struct ModInt_Exp<1> {
 using mod_t= ModInt_Exp;
 bool a;
 constexpr ModInt_Exp(): a(0) {}
 constexpr ModInt_Exp(uint64_t x): a(x) {}
 constexpr uint32_t val() { return 0; }
 constexpr mod_t &operator*=(const mod_t &r) { return a&= r.a, *this; }
 constexpr mod_t &operator+=(const mod_t &r) { return a|= r.a, *this; }
 constexpr mod_t operator*(const mod_t &r) const { return mod_t(*this)*= r; }
 constexpr mod_t operator+(const mod_t &r) const { return mod_t(*this)+= r; }
 constexpr mod_t pow(const mod_t &r) const { return {a || !r.a}; };
};
template <uint64_t MOD> std::ostream &operator<<(std::ostream &os, const ModInt_Exp<MOD> &r) { return os << r.val(); }
template <uint64_t MOD> std::istream &operator>>(std::istream &is, ModInt_Exp<MOD> &r) {
 uint64_t v;
 return is >> v, r= ModInt_Exp<MOD>(v), is;
}
