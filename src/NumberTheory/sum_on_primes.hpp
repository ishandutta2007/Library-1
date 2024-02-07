#pragma once
#include "src/NumberTheory/enumerate_primes.hpp"
#include "src/NumberTheory/CumSumQuotient.hpp"
template <class T> std::vector<CumSumQuotient<T>> sums_of_powers_on_primes(uint64_t N, size_t D) {
 size_t K= std::sqrt(N);
 std::vector ret(D + 1, CumSumQuotient<T>(N));
 for (int n= 1, d= 0; n <= K; ++n, d= 0)
  for (T prd= n; d <= D; prd*= (n + ++d)) ret[d].X[n]= prd / (d + 1);
 for (int n= 1, d= 0; n <= K; ++n, d= 0)
  for (T prd= N / n; d <= D; prd*= ((N / n) + ++d)) ret[d].X[n + K]= prd / (d + 1);
 if (D >= 2) {
  std::vector<T> stir(D + 1, 0);
  stir[1]= 1;
  for (size_t d= 2; d <= D; stir[d++]= 1) {
   for (size_t j= d; --j;) stir[j]= stir[j - 1] + stir[j] * (d - 1);
   for (size_t j= 1; j < d; ++j) ret[d].X-= stir[j] * ret[j].X;
  }
 }
 for (size_t d= 0; d <= D; ++d) ret[d].X-= 1;
 for (int p: enumerate_primes(K)) {
  uint64_t q= uint64_t(p) * p, M= N / p;
  T pw= 1;
  for (size_t d= 0, t= K / p, u= std::min<uint64_t>(K, N / q); d <= D; ++d, pw*= p) {
   auto &X= ret[d].X;
   T tk= X[p - 1];
   for (size_t n= 1; n <= t; ++n) X[n + K]-= (X[n * p + K] - tk) * pw;
   for (size_t n= t + 1; n <= u; ++n) X[n + K]-= (X[double(M) / n] - tk) * pw;
   for (uint64_t n= K; n >= q; --n) X[n]-= (X[double(n) / p] - tk) * pw;
  }
 }
 return ret;
}
template <class T, class F> T additive_sum(const CumSumQuotient<T> &P, const F &f) {
 T ret= P.sum();
 for (uint64_t d= 2, nN, nd; nN; d= nd) ret+= P(nN= double(P.N) / d) * ((nd= double(P.N) / nN + 1) - d);
 for (uint64_t p: enumerate_primes(P.K))
  for (uint64_t pw= p * p, e= 2; pw <= P.N; ++e, pw*= p) ret+= (f(p, e) - f(p, e - 1)) * (P.N / pw);
 return ret;
}
template <class T, class F> T multiplicative_sum(CumSumQuotient<T> P, const F &f) {
 auto ps= enumerate_primes(P.K);
 size_t psz= ps.size();
 for (size_t j= psz; j--;) {
  uint64_t p= ps[j], M= P.N / p, q= p * p;
  size_t t= P.K / p, u= std::min<uint64_t>(P.K, P.N / q);
  T tk= P.X[p - 1];
  for (auto i= q; i <= P.K; ++i) P.X[i]+= (P.X[double(i) / p] - tk) * f(p, 1);
  for (size_t i= u; i > t; --i) P.X[i + P.K]+= (P.X[double(M) / i] - tk) * f(p, 1);
  for (size_t i= t; i; --i) P.X[i + P.K]+= (P.X[i * p + P.K] - tk) * f(p, 1);
 }
 P.X+= 1;
 auto dfs= [&](auto &rc, uint64_t n, size_t bg, T cf) -> T {
  if (cf == T(0)) return T(0);
  T ret= cf * P(n);
  for (auto i= bg; i < psz; ++i) {
   uint64_t p= ps[i], q= p * p, nn= n / q;
   if (!nn) break;
   for (int e= 2; nn; nn/= p, ++e) ret+= rc(rc, nn, i + 1, cf * (f(p, e) - f(p, 1) * f(p, e - 1)));
  }
  return ret;
 };
 return dfs(dfs, P.N, 0, 1);
}