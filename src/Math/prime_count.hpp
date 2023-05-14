#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
template <class T> class QuotientSum {
 uint64_t N;
 size_t K;
public:
 std::vector<T> s, l;
 QuotientSum() {}
 QuotientSum(uint64_t N, size_t K, const std::vector<T> &s, const std::vector<T> &l): N(N), K(K), s(s), l(l) {}
 inline T sum() const { return l[1]; }
 inline T sum(uint64_t n) const { return n <= K ? s[n] : l[N / n]; }
};
template <class T= __int128_t> auto polynomial_prime_sum_table(uint64_t N, const std::vector<T> &poly) {
 const int sqrtN= std::sqrt(N), d= poly.size();
 std::vector<int> primes;
 std::vector<std::vector<T>> s(d, std::vector<T>(sqrtN + 1)), l(d, std::vector<T>(sqrtN + 1));
 for (int n= 1, k= 0; n <= sqrtN; ++n, k= 0)
  for (T prd= n; k < d; prd*= (n + ++k)) s[k][n]= prd / (k + 1);
 for (int n= 1, k= 0; n <= sqrtN; ++n, k= 0)
  for (T prd= N / n; k < d; prd*= ((N / n) + ++k)) l[k][n]= prd / (k + 1);
 if (d > 2) {
  std::vector<T> stir(d, 0);
  stir[1]= 1;
  for (int k= 2; k < d; stir[k++]= 1) {
   for (int j= k - 1; j; --j) stir[j]= stir[j - 1] + stir[j] * (k - 1);
   for (int n= 1; n <= sqrtN; ++n)
    for (int j= 1; j < k; ++j) s[k][n]-= stir[j] * s[j][n], l[k][n]-= stir[j] * l[j][n];
  }
 }
 for (int k= 0; k < d; ++k)
  for (int n= 1; n <= sqrtN; ++n) s[k][n]-= 1, l[k][n]-= 1;
 for (int p= 2, k= 0; p <= sqrtN; ++p, k= 0)
  if (s[0][p] != s[0][p - 1]) {
   primes.emplace_back(p);
   uint64_t q= uint64_t(p) * p, M= N / p;
   int t= sqrtN / p, u= std::min<uint64_t>(sqrtN, N / q);
   for (T pw= 1; k < d; ++k, pw*= p)
    if (!k || poly[k] != T(0)) {
     T tk= s[k][p - 1];
     for (int i= 1; i <= t; ++i) l[k][i]-= (l[k][i * p] - tk) * pw;
     for (int i= t + 1; i <= u; ++i) l[k][i]-= (s[k][double(M) / i] - tk) * pw;
     for (uint64_t i= sqrtN; i >= q; --i) s[k][i]-= (s[k][double(i) / p] - tk) * pw;
    }
  }
 std::vector<T> Xs(sqrtN + 1, 0), Xl(sqrtN + 1, 0);
 for (int n= 1; n <= sqrtN; ++n)
  for (int k= 0; k < d; ++k) Xs[n]+= s[k][n] * poly[k], Xl[n]+= l[k][n] * poly[k];
 return std::make_pair(primes, QuotientSum(N, sqrtN, Xs, Xl));
}
auto prime_count_table(uint64_t N) { return polynomial_prime_sum_table<uint64_t>(N, {1}); }
template <class T, class F> T additive_sum(uint64_t N, const F &f, std::vector<T> poly) {
 const uint64_t sqrtN= std::sqrt(N);
 auto [primes, X]= polynomial_prime_sum_table<T>(N, poly);
 T ret= X.sum();
 for (uint64_t d= 2, nN, nd; nN; d= nd) ret+= X.sum(nN= double(N) / d) * ((nd= N / nN + 1) - d);
 for (uint64_t p: primes)
  for (uint64_t pw= p * p, e= 2; pw <= N; ++e, pw*= p) ret+= (f(p, e) - f(p, e - 1)) * uint64_t(double(N) / pw);
 return ret;
}
template <class T= __int128_t, class F> T multiplicative_sum(uint64_t N, const F &f, const std::vector<T> &poly) {
 const uint64_t sqrtN= std::sqrt(N);
 std::vector<int> primes;
 QuotientSum<T> X;
 tie(primes, X)= polynomial_prime_sum_table<T>(N, poly);
 size_t psz= primes.size();
 for (auto it= primes.rbegin(); it != primes.rend(); ++it) {
  uint64_t p= *it, M= N / p, q= p * p;
  int t= sqrtN / p, u= std::min(sqrtN, N / q);
  T tk= X.s[p - 1];
  for (auto i= q; i <= sqrtN; ++i) X.s[i]+= (X.s[double(i) / p] - tk) * f(p, 1);
  for (int i= u; i > t; --i) X.l[i]+= (X.s[double(M) / i] - tk) * f(p, 1);
  for (int i= t; i; --i) X.l[i]+= (X.l[i * p] - tk) * f(p, 1);
 }
 for (auto n= sqrtN; n; --n) X.s[n]+= 1;
 for (auto n= sqrtN; n; --n) X.l[n]+= 1;
 auto dfs= [&](auto rc, uint64_t n, size_t bg, T cf) -> T {
  if (cf == T(0)) return T(0);
  T ret= cf * X.sum(n);
  for (auto i= bg; i < psz; ++i) {
   uint64_t p= primes[i], q= p * p, nn= double(n) / q;
   if (!nn) break;
   for (int e= 2; nn; nn= double(nn) / p, ++e) ret+= rc(rc, nn, i + 1, cf * (f(p, e) - f(p, 1) * f(p, e - 1)));
  }
  return ret;
 };
 return dfs(dfs, N, 0, 1);
}