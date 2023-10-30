#pragma once
#include <algorithm>
#include <vector>
#include <cassert>
#include <cstdint>
template <unsigned short MAX_N= 21> struct SetPowerSeries {
#define SUBSET_REP(i, j, n) \
 for (int _= (n); _>>= 1;) \
  for (int __= 0, _2= _ << 1; __ < (n); __+= _2) \
   for (int j= __, i= j | _, ___= i; j < ___; ++j, ++i)
 template <typename T> static inline void ranked_zeta_tr(const T f[], T ret[][MAX_N + 1], const int sz) {
  for (int S= sz, c; S--;) ret[S][c= __builtin_popcount(S)]= f[S], std::fill_n(ret[S], c, 0);
  SUBSET_REP(S, U, sz)
  for (int d= __builtin_popcount(S); d--;) ret[S][d]+= ret[U][d];
 }
 template <typename T> static inline void conv_na(const T f[], const T g[], T ret[], const int sz) {
  for (int s= sz, t; s--;)
   for (ret[t= s]= f[s] * g[0]; t; --t&= s) ret[s]+= f[s ^ t] * g[t];
 }
 template <typename T> static inline void conv_tr(const T f[], const T g[], T ret[], const int sz) {
  static T F[1 << MAX_N][MAX_N + 1], G[1 << MAX_N][MAX_N + 1];
  T tmp[MAX_N + 1];
  ranked_zeta_tr(f, F, sz), ranked_zeta_tr(g, G, sz);
  const int n= __builtin_ctz(sz);
  for (int S= sz, c, d, e, bg; S--;) {
   c= __builtin_popcount(S), bg= std::min(2 * c, n);
   for (d= bg; d >= c; d--)
    for (tmp[d]= 0, e= d - c; e <= c; ++e) tmp[d]+= F[S][e] * G[S][d - e];
   for (d= bg; d >= c; d--) F[S][d]= tmp[d];
  }
  SUBSET_REP(S, U, sz)
  for (int c= __builtin_popcount(U), d= std::min(2 * c, n); d > c; d--) F[S][d]-= F[U][d];
  for (int S= sz; S--;) ret[S]= F[S][__builtin_popcount(S)];
 }
 template <typename T, class F> static inline void onconv_na(const T g[], T ret[], const F &phi, const int sz) {
  for (int s= 1, t; s < sz; phi(s, ret[s]), ++s)
   for (ret[t= s]= 0; t; --t&= s) ret[s]+= ret[s ^ t] * g[t];
 }
 template <typename T, class F> static inline void onconv_tr(const T g[], T ret[], const F &phi, const int sz) {
  static T G[1 << MAX_N][MAX_N + 1], mat[MAX_N + 1][1 << MAX_N];
  const int n= __builtin_ctz(sz);
  ranked_zeta_tr(g, G, sz), std::fill_n(mat[0], sz, ret[0]);
  for (int d= n; d; d--) std::fill_n(mat[d], sz, 0);
  for (int I= sz; I>>= 1;) phi(I, mat[1][I]= ret[0] * g[I]);
  for (int d= 2; d <= n; ++d) {
   SUBSET_REP(S, U, sz) mat[d - 1][S]+= mat[d - 1][U];
   for (int S= sz; S--;)
    if (int c= __builtin_popcount(S); c <= d && d <= 2 * c)
     for (int e= d; e--;) mat[d][S]+= mat[e][S] * G[S][d - e];
   SUBSET_REP(S, U, sz) mat[d][S]-= mat[d][U];
   for (int S= sz; S--;) __builtin_popcount(S) == d ? phi(S, mat[d][S]), 0 : (mat[d][S]= 0);
  }
  for (int S= sz; --S;) ret[S]= mat[__builtin_popcount(S)][S];
 }
public:
 template <typename T>  // O(n 2^n)
 static inline void subset_sum(std::vector<T> &f) {
  SUBSET_REP(S, U, f.size()) f[S]+= f[U];
 }
 template <typename T>  // O(n 2^n)
 static inline void subset_sum_inv(std::vector<T> &f) {
  SUBSET_REP(S, U, f.size()) f[S]-= f[U];
 }
 template <class T>  // O(n^2 2^n)
 static inline std::vector<T> convolve(const std::vector<T> &f, const std::vector<T> &g) {
  const int sz= f.size(), n= __builtin_ctz(sz);
  std::vector<T> ret(sz);
  if (n <= 10) return conv_na(f.data(), g.data(), ret.data(), sz), ret;
  assert(sz == 1 << n && sz == g.size());
  return conv_tr(f.data(), g.data(), ret.data(), sz), ret;
 }
 // f(S) = φ_S ( Σ_{T⊊S} f(T)g(S/T) )
 template <class T, class F= void (*)(int, T &)>  // O(n^2 2^n)
 static inline std::vector<T> semi_relaxed_convolve(
     const std::vector<T> &g, T init, const F &phi= [](int, T &) {}) {
  const int sz= g.size(), n= __builtin_ctz(sz);
  std::vector<T> ret(sz);
  ret[0]= init;
  if (n <= 12) return onconv_na(g.data(), ret.data(), phi, sz), ret;
  assert(sz == 1 << n);
  return onconv_tr(g.data(), ret.data(), phi, sz), ret;
 }
 // f(S) = φ_S ( Σ_{∅≠T⊊S & (T<(S/T) as binary numbers) } f(T)f(S/T) )
 template <class T, class F>  // O(n^2 2^n)
 static inline std::vector<T> self_relaxed_convolve(int n, const F &phi) {
  assert(__builtin_popcount(n) == 1);
  int I= 1, ed= std::min(1 << 13, n);
  std::vector<T> ret(n, 0);
  for (int s, t, u= 1; I < ed; I<<= 1)
   for (t= s= 0; s < I; phi(u, ret[u]), t= ++s, ++u)
    for (ret[u]= 0; t; --t&= s) ret[u]+= ret[u ^ t] * ret[t];
  T *h= ret.data();
  for (; I < n; I<<= 1)
   phi(I, ret[I]), onconv_tr(
                       h, h + I, [&](int s, T &x) { phi(s | I, x); }, I);
  return ret;
 }
 // F(f) : F[i] is coefficient of EGF ( = F^{(i)}(0) )
 // "f[∅] = 0" is required.
 template <class T, class EGF>  // O(n^2 2^n)
 static inline std::vector<T> composite(const std::vector<T> &f, const EGF &F) {
  const int sz= f.size(), m= __builtin_ctz(sz), sz2= sz >> 1;
  assert(sz == 1 << m), assert(f.at(0) == 0);
  std::vector<T> ret(sz);
  T *h= ret.data() + sz;
  const T *g= f.data();
  for (int i= 0; i <= m; ++i) ret[sz - (1 << i)]= F[m - i];
  int l= 1, ed= std::min(sz, 1 << 11), j;
  for (; l < ed; l<<= 1)
   for (j= sz2; j >= l; j>>= 1) conv_na(h - j, g + l, h - j - j + l, l);
  for (; l < sz; l<<= 1)
   for (j= sz2; j >= l; j>>= 1) conv_tr(h - j, g + l, h - j - j + l, l);
  return ret;
 }
 // exp(f) : "f[∅] = 0" is required.
 template <class T>  // O(n^2 2^n)
 static inline std::vector<T> exp(const std::vector<T> &f) {
  const int sz= f.size();
  assert(!(sz & (sz - 1))), assert(f.at(0) == 0);
  T h[sz];
  const T *g= f.data();
  int l= 1, ed= std::min(sz, 1 << 11);
  for (h[0]= 1; l < ed; l<<= 1) conv_na(h, g + l, h + l, l);
  for (; l < sz; l<<= 1) conv_tr(h, g + l, h + l, l);
  return std::vector<T>(h, h + sz);
 }
 // log(f) : "f[∅] = 1" is required.
 template <class T>  // O(n^2 2^n)
 static inline std::vector<T> log(const std::vector<T> &f) {
  const int sz= f.size();
  assert(!(sz & (sz - 1))), assert(f.at(0) == T(1));
  int I= 2, ed= std::min(sz, 1 << 13);
  T h[sz];
  const T *g= f.data();
  for (std::copy_n(g, ed, h); I < ed; I<<= 1)
   for (int s= 1, u= s | I; s < I; ++s, ++u)
    for (int t= s; t; --t&= s) h[u]-= h[u ^ t] * f[t];
  for (; I < sz; I<<= 1)
   h[I]= g[I], onconv_tr(
                   g, h + I, [&](int s, T &x) { x= g[I | s] - x; }, I);
  return h[0]= 0, std::vector<T>(h, h + sz);
 }
 // f^k
 template <class T>  // O(n^2 2^n)
 static inline std::vector<T> pow(std::vector<T> f, uint64_t k) {
  const int sz= f.size(), n= __builtin_ctz(sz);
  assert(sz == 1 << n);
  T F[MAX_N + 1]= {1}, pw= 1, bs= f[0];
  int i= 1, ed= std::min<uint64_t>(n, k);
  for (; i <= ed; ++i) F[i]= F[i - 1] * (k - i + 1);
  for (auto e= k - --i; e; e>>= 1, bs*= bs)
   if (e & 1) pw*= bs;
  for (; i >= 0; --i, pw*= f[0]) F[i]*= pw;
  return f[0]= 0, composite(f, F);
 }
 // P(f), P is polynomial
 template <class T> static inline std::vector<T> polynomial_composite(std::vector<T> f, std::vector<T> P) {
  const int sz= f.size(), n= __builtin_ctz(sz);
  assert(sz == 1 << n);
  T F[MAX_N + 1]= {};
  int e= P.size();
  if (!e) return std::vector<T>(sz);
  for (int j= 0;; ++j, --e) {
   for (int i= e; i--;) (F[j]*= f[0])+= P[i];
   if (j == n || e == 1) break;
   for (int i= 1; i < e; ++i) P[i - 1]= P[i] * i;
  }
  return f[0]= 0, composite(f, F);
 }
 // {[X^{[n]}]f^k/k!} for k=0,1,...,n
 template <class T>  // O(n^2 2^n)
 static inline std::vector<T> egf(std::vector<T> f) {
  static constexpr int M= 1 << 11;
  const int sz= f.size(), n= __builtin_ctz(sz), sz4= sz >> 2;
  assert(sz == 1 << n);
  if (n == 1) return {0, f[1]};
  int l= sz4, m;
  T *in= f.data() + l, *dp= in + l, tmp[sz4 / 2], *dp2;
  for (int s; l > M; conv_tr(dp, in, dp, l), in-= (l>>= 1))
   for (m= sz4; dp2= dp + (m - l), m > l; m>>= 1)
    for (s= l, conv_tr(dp2 + m - l, in, tmp, l); s--;) dp2[s]+= tmp[s];
  for (int s; l; conv_na(dp, in, dp, l), in-= (l>>= 1))
   for (m= sz4; dp2= dp + (m - l), m > l; m>>= 1)
    for (s= l, conv_na(dp2 + m - l, in, tmp, l); s--;) dp2[s]+= tmp[s];
  std::vector<T> ret(n + 1, 0);
  for (int i= n + 1; --i;) ret[i]= dp[(1 << (n - i)) - 1];
  return ret;
 }
 // {[X^{[n]}]g*f^k/k!} for k=0,1,...,n
 template <class T>  // O(n^2 2^n)
 static inline std::vector<T> egf(const std::vector<T> &f, std::vector<T> g) {
  static constexpr int M= 1 << 11;
  const int sz= f.size(), n= __builtin_ctz(sz), sz2= sz >> 1, sz4= sz >> 2;
  assert(sz == 1 << n), assert(sz == (int)g.size());
  if (n == 1) return {g[1], f[1] * g[0] + f[0] * g[1]};
  int l= sz2, m;
  const T *in= f.data() + sz2;
  T *dp= g.data(), tmp[sz2 / 2], *dp2;
  for (int s; l > M; conv_tr(dp, in, dp, l), in-= (l>>= 1))
   for (m= sz2; dp2= dp + (m - l), m > l; m>>= 1)
    for (s= l, conv_tr(dp2 + m - l, in, tmp, l); s--;) dp2[s]+= tmp[s];
  for (int s; l; conv_na(dp, in, dp, l), in-= (l>>= 1))
   for (m= sz2; dp2= dp + (m - l), m > l; m>>= 1)
    for (s= l, conv_na(dp2 + m - l, in, tmp, l); s--;) dp2[s]+= tmp[s];
  std::vector<T> ret(n + 1);
  for (int i= n + 1; i--;) ret[i]= dp[(1 << (n - i)) - 1];
  return ret;
 }
#undef SUBSET_REP
};