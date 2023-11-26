#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <cstdint>
#include <cassert>
template <class T> struct PrimeSum {
 uint64_t N;
 size_t K;
 std::vector<T> Xs, Xl;
 PrimeSum(uint64_t N= 0): N(N), K(std::sqrt(N)), Xs(K + 1), Xl(K + 1) {}
 PrimeSum(uint64_t N, const std::vector<T> &s, const std::vector<T> &l): N(N), K(std::sqrt(N)), Xs(s), Xl(l) {}
 PrimeSum operator-() const {
  auto Ys= Xs, Yl= Xl;
  for (auto &x: Ys) x= -x;
  for (auto &x: Yl) x= -x;
  return PrimeSum(N, Ys, Yl);
 }
 PrimeSum &operator*=(T r) {
  for (auto &x: Xs) x*= r;
  for (auto &x: Xl) x*= r;
  return *this;
 }
 PrimeSum &operator+=(const PrimeSum &r) {
  for (size_t i= K + 1; i--;) Xs[i]+= r.Xs[i];
  for (size_t i= K + 1; i--;) Xl[i]+= r.Xl[i];
  return *this;
 }
 PrimeSum &operator-=(const PrimeSum &r) {
  for (size_t i= K + 1; i--;) Xs[i]-= r.Xs[i];
  for (size_t i= K + 1; i--;) Xl[i]-= r.Xl[i];
  return *this;
 }
 PrimeSum operator*(T r) const { return PrimeSum(*this)*= r; }
 friend PrimeSum operator*(T l, const PrimeSum &r) { return r * l; }
 PrimeSum operator+(const PrimeSum &r) const { return PrimeSum(*this)+= r; }
 PrimeSum operator-(const PrimeSum &r) const { return PrimeSum(*this)-= r; }
 inline T sum() const { return Xl[1]; }
 inline T sum(uint64_t n) const { return n <= K ? Xs[n] : Xl[N / n]; }
 void add(uint64_t p, T v) {
  for (size_t i= p; i <= K; ++i) Xs[i]+= v;
  for (size_t i= std::min<uint64_t>(N / p, K); i; --i) Xl[i]+= v;
 }
};
template <class T> class ExtendedEratosthenesSieve {
 uint64_t N;
 size_t K;
public:
 std::vector<size_t> primes;
 std::vector<PrimeSum<T>> pwsum;
 ExtendedEratosthenesSieve(uint64_t N, size_t D): N(N), K(std::sqrt(N)), pwsum(D + 1) {
  std::vector<std::vector<T>> s(D + 1, std::vector<T>(K + 1)), l(D + 1, std::vector<T>(K + 1));
  T fct= 1;
  for (size_t d= D + 1; d; --d) fct*= d;
  fct= T(1) / fct;
  for (size_t n= 1, d= 0; n <= K; ++n, d= 0)
   for (T prd= n; d <= D; prd*= (n + ++d)) s[d][n]= prd;
  for (size_t n= 1, d= 0; n <= K; ++n, d= 0)
   for (T prd= N / n; d <= D; prd*= ((N / n) + ++d)) l[d][n]= prd;
  for (size_t d= D + 1; --d; fct*= d + 1)
   for (size_t n= 1; n <= K; ++n) s[d][n]*= fct, l[d][n]*= fct;
  if (D >= 2) {
   std::vector<T> stir(D + 1, 0);
   stir[1]= 1;
   for (size_t d= 2; d <= D; stir[d++]= 1) {
    for (size_t j= d; --j;) stir[j]= stir[j - 1] + stir[j] * (d - 1);
    for (size_t n= 1; n <= K; ++n)
     for (size_t j= 1; j < d; ++j) s[d][n]-= stir[j] * s[j][n], l[d][n]-= stir[j] * l[j][n];
   }
  }
  for (size_t d= 0; d <= D; ++d)
   for (size_t n= 1; n <= K; ++n) s[d][n]-= 1, l[d][n]-= 1;
  for (size_t p= 2, d= 0; p <= K; ++p, d= 0)
   if (s[0][p] != s[0][p - 1]) {
    primes.emplace_back(p);
    uint64_t q= uint64_t(p) * p, M= N / p;
    int t= K / p, u= std::min<uint64_t>(K, N / q);
    for (T pw= 1; d <= D; ++d, pw*= p) {
     T tk= s[d][p - 1];
     for (int n= 1; n <= t; ++n) l[d][n]-= (l[d][n * p] - tk) * pw;
     for (int n= t + 1; n <= u; ++n) l[d][n]-= (s[d][M / n] - tk) * pw;
     for (uint64_t n= K; n >= q; --n) s[d][n]-= (s[d][n / p] - tk) * pw;
    }
   }
  for (size_t d= 0; d <= D; ++d) pwsum[d]= PrimeSum(N, s[d], l[d]);
 }
 template <class F> T additive_sum(const F &f, const PrimeSum<T> &X) const {
  T ret= X.sum();
  for (uint64_t d= 2, nN, nd; nN; d= nd) ret+= X.sum(nN= N / d) * ((nd= N / nN + 1) - d);
  for (uint64_t p: primes)
   for (uint64_t pw= p * p, e= 2; pw <= N; ++e, pw*= p) ret+= (f(p, e) - f(p, e - 1)) * (N / pw);
  return ret;
 }
 template <class F> T additive_sum(const F &f, const std::vector<T> &poly) const {
  PrimeSum<T> X(N);
  assert(poly.size() <= pwsum.size());
  for (size_t d= poly.size(); d--;) X+= poly[d] * pwsum[d];
  return additive_sum(f, X);
 }
 template <class F> T multiplicative_sum(const F &f, PrimeSum<T> X) const {
  size_t psz= primes.size();
  for (size_t j= psz; j--;) {
   uint64_t p= primes[j], M= N / p, q= p * p;
   size_t t= K / p, u= std::min<uint64_t>(K, N / q);
   T tk= X.Xs[p - 1];
   for (auto i= q; i <= K; ++i) X.Xs[i]+= (X.Xs[i / p] - tk) * f(p, 1);
   for (size_t i= u; i > t; --i) X.Xl[i]+= (X.Xs[M / i] - tk) * f(p, 1);
   for (size_t i= t; i; --i) X.Xl[i]+= (X.Xl[i * p] - tk) * f(p, 1);
  }
  for (auto n= K; n; --n) X.Xs[n]+= 1;
  for (auto n= K; n; --n) X.Xl[n]+= 1;
  auto dfs= [&](auto &rc, uint64_t n, size_t bg, T cf) -> T {
   if (cf == T(0)) return T(0);
   T ret= cf * X.sum(n);
   for (auto i= bg; i < psz; ++i) {
    uint64_t p= primes[i], q= p * p, nn= n / q;
    if (!nn) break;
    for (int e= 2; nn; nn/= p, ++e) ret+= rc(rc, nn, i + 1, cf * (f(p, e) - f(p, 1) * f(p, e - 1)));
   }
   return ret;
  };
  return dfs(dfs, N, 0, 1);
 }
 template <class F> T multiplicative_sum(const F &f, const std::vector<T> &poly) const {
  PrimeSum<T> X(N);
  assert(poly.size() <= pwsum.size());
  for (size_t d= poly.size(); d--;) X+= poly[d] * pwsum[d];
  return multiplicative_sum(f, X);
 }
};