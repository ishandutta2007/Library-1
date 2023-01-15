#pragma once
#include <bits/stdc++.h>
namespace math_internal {
using namespace std;
using u8= uint8_t;
using u32= uint32_t;
using u64= uint64_t;
using i64= int64_t;
using u128= __uint128_t;
#define CE constexpr
#define IL inline
#define NORM \
 if (n >= mod) n-= mod; \
 return n
#define PLUS(U, M) \
 CE IL U plus(U l, U r) const { \
  if (l+= r; l >= M) l-= M; \
  return l; \
 }
#define DIFF(U, C, M) \
 CE IL U diff(U l, U r) const { \
  if (l-= r; l >> C) l+= M; \
  return l; \
 }
#define SGN(U) \
 static CE IL U set(U n) { return n; } \
 static CE IL U get(U n) { return n; } \
 static CE IL U norm(U n) { return n; }
template <class u_t, class du_t, u8 B, u8 A> struct MP_Mo {
 const u_t mod;
 CE MP_Mo(): mod(0), iv(0), r2(0) {}
 CE MP_Mo(u_t m): mod(m), iv(inv(m)), r2(-du_t(mod) % mod) {}
 CE IL u_t mul(u_t l, u_t r) const { return reduce(du_t(l) * r); }
 PLUS(u_t, mod << 1)
 DIFF(u_t, A, mod << 1)
 CE IL u_t set(u_t n) const { return mul(n, r2); }
 CE IL u_t get(u_t n) const {
  n= reduce(n);
  NORM;
 }
 CE IL u_t norm(u_t n) const { NORM; }
private:
 const u_t iv, r2;
 static CE u_t inv(u_t n, int e= 6, u_t x= 1) { return e ? inv(n, e - 1, x * (2 - x * n)) : x; }
 CE IL u_t reduce(const du_t &w) const { return u_t(w >> B) + mod - ((du_t(u_t(w) * iv) * mod) >> B); }
};
struct MP_Na {
 const u32 mod;
 CE MP_Na(): mod(0){};
 CE MP_Na(u32 m): mod(m) {}
 CE IL u32 mul(u32 l, u32 r) const { return u64(l) * r % mod; }
 PLUS(u32, mod) DIFF(u32, 31, mod) SGN(u32)
};
struct MP_Br {  // mod < 2^31
 const u32 mod;
 CE MP_Br(): mod(0), s(0), x(0) {}
 CE MP_Br(u32 m): mod(m), s(__lg(m - 1) + 64), x(((u128(1) << s) + m - 1) / m) {}
 CE IL u32 mul(u32 l, u32 r) const { return rem(u64(l) * r); }
 PLUS(u32, mod) DIFF(u32, 31, mod) SGN(u32) private: const u8 s;
 const u64 x;
 CE IL u64 quo(u64 n) const { return (u128(x) * n) >> s; }
 CE IL u32 rem(u64 n) const { return n - quo(n) * mod; }
};
struct MP_Br2 {  // 2^20 < mod <= 2^41
 const u64 mod;
 CE MP_Br2(): mod(0), x(0) {}
 CE MP_Br2(u64 m): mod(m), x((u128(1) << 84) / m) {}
 CE IL u64 mul(u64 l, u64 r) const { return rem(u128(l) * r); }
 PLUS(u64, mod << 1)
 DIFF(u64, 63, mod << 1)
 static CE IL u64 set(u64 n) { return n; }
 CE IL u64 get(u64 n) const { NORM; }
 CE IL u64 norm(u64 n) const { NORM; }
private:
 const u64 x;
 CE IL u128 quo(const u128 &n) const { return (n * x) >> 84; }
 CE IL u64 rem(const u128 &n) const { return n - quo(n) * mod; }
};
struct MP_D2B1 {
 const u64 mod;
 CE MP_D2B1(): mod(0), s(0), d(0), v(0) {}
 CE MP_D2B1(u64 m): mod(m), s(__builtin_clzll(m)), d(m << s), v(u128(-1) / d) {}
 CE IL u64 mul(u64 l, u64 r) const { return rem((u128(l) * r) << s) >> s; }
 PLUS(u64, mod) DIFF(u64, 63, mod) SGN(u64) private: CE IL u64 rem(const u128 &u) const {
  u128 q= (u >> 64) * v + u;
  u64 r= u64(u) - (q >> 64) * d - d;
  if (r > u64(q)) r+= d;
  if (r >= d) r-= d;
  return r;
 }
 const u8 s;
 const u64 d, v;
};
template <class u_t, class MP> CE u_t pow(u_t x, u64 k, const MP &md) {
 for (u_t ret= md.set(1);; x= md.mul(x, x))
  if (k & 1 ? ret= md.mul(ret, x) : 0; !(k>>= 1)) return ret;
}
#undef NORM
#undef PLUS
#undef DIFF
#undef SGN
#undef CE
}