#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <cstdint>
template <int LIM= 1 << 24> class Sieve {
 static inline int ps[LIM >> 4], lpf[LIM >> 1], lpfpw[LIM >> 1], psz= 0;
 static inline int8_t lpfe[LIM >> 1];
 static inline void sieve(int N) {  // O(N)
  static int n= 2, i= 1;
  if (n == 2) ps[psz++]= 2, n++;
  for (; n <= N; n+= 2, i++) {
   if (!lpf[i]) lpf[i]= ps[psz++]= n;
   for (int j= 1, e= std::min(lpf[i], N / n); j < psz && ps[j] <= e; j++) lpf[(ps[j] * n) >> 1]= ps[j];
  }
 }
 static inline void set_lpfe(int N) {  // O(N)
  static int n= 3, i= 1;
  if (N < n) return;
  sieve(N), std::copy(lpf + i, lpf + (N >> 1) + 1, lpfpw + i);
  for (std::fill(lpfe + i, lpfe + (N >> 1) + 1, 1); n <= N; n+= 2, i++)
   if (int j= (n / lpf[i]) >> 1; lpf[i] == lpf[j]) lpfe[i]+= lpfe[j], lpfpw[i]*= lpfpw[j];
 }
public:
 static int least_prime_factor(int n) { return sieve(n), lpf[n]; }
 // O(log n)
 static std::map<int, short> factorize(int n) {
  std::map<int, short> ret;
  if (int t; !(n & 1)) ret[2]= t= __builtin_ctz(n), n>>= t;
  if (int i= n >> 1; n > 1)
   for (set_lpfe(n); n > 1; i= n >> 1) ret[lpf[i]]= lpfe[i], n/= lpfpw[i];
  return ret;
 }
 // O(log n)
 static std::vector<int> divisors(int n) {
  std::vector<int> ret= {1};
  for (auto [p, e]: factorize(n))
   for (std::size_t sz= ret.size(), pw= p; e--; pw*= p)
    for (int i= sz - 1; i >= 0; i--) ret.push_back(ret[i] * pw);
  return std::sort(ret.begin(), ret.end()), ret;
 }
 // O(N)
 static std::vector<int> get_primes(int N) { return sieve(N), std::vector<int>(ps, std::upper_bound(ps, ps + psz, N)); }
 template <class T, class F> static inline std::vector<T> completely_multiplicative_table(int N, const F &f) {
  std::vector<T> ret(N + 1);
  sieve(N);
  for (int n= 3, i= 1; n <= N; n+= 2, i++) ret[n]= lpf[i] == n ? f(n, 1) : ret[lpf[i]] * ret[n / lpf[i]];
  if (int n= 4; 2 <= N)
   for (ret[2]= f(2, 1); n <= N; n+= 2) ret[n]= ret[2] * ret[n >> 1];
  return ret[1]= 1, ret;
 }
 template <class T, class F> static inline std::vector<T> multiplicative_table(int N, const F &f) {
  std::vector<T> ret(N + 1);
  set_lpfe(N);
  for (int n= 3, i= 1; n <= N; n+= 2, i++) ret[n]= lpfpw[i] == n ? f(lpf[i], lpfe[i]) : ret[lpfpw[i]] * ret[n / lpfpw[i]];
  for (int n= 2, t; n <= N; n+= 2) t= __builtin_ctz(n), ret[n]= n & (n - 1) ? ret[n & -n] * ret[n >> t] : f(2, t);
  return ret[1]= 1, ret;
 }
 // O(N log k / log N + N)
 template <class T> static std::vector<T> pow_table(int N, std::uint64_t k) {
  if (k == 0) return std::vector<T>(N + 1, 1);
  auto f= [k](int p, short) {
   T ret= 1, b= p;
   for (auto e= k;; b*= b)
    if (e & 1 ? ret*= b, !(e>>= 1) : !(e>>= 1)) return ret;
  };
  return completely_multiplicative_table<T>(N, f);
 }
 // O(N log MOD / log N + N)
 template <class T> static std::vector<T> inv_table(int N) {
  return completely_multiplicative_table<T>(N, [](int p, short) { return T(1) / p; });
 }
 // naive , O(N log N)
 template <class T> static std::vector<T> dirichlet_conv(const std::vector<T> &a, const std::vector<T> &b) {
  std::size_t N= std::max(a.size(), b.size()) - 1, i, j;
  std::vector<T> ret(N + 1, 0);
  for (i= a.size(); --i;)
   for (j= std::min(b.size() - 1, N / i); j; j--) ret[i * j]+= a[i] * b[j];
  return ret;
 }
 // a is multiplicative, O(N log log N)
 template <class T, class F> static std::vector<T> dirichlet_conv(const F &a, std::vector<T> b) {
  std::size_t N= b.size() - 1, j= 0;
  for (sieve(N), b.resize(N + 1, 0); j < psz && ps[j] <= N; j++)
   for (int i= N / ps[j], n, m, e; i; i--)
    for (b[n= ps[j] * i]+= a(ps[j], e= 1) * b[m= i]; m % ps[j] == 0;) b[n]+= a(ps[j], ++e) * b[m/= ps[j]];
  return b;
 }
 // both a and b are multiplicative, O(N)
 template <class T, class F1, class F2> static std::vector<T> dirichlet_conv(const F1 &a, const F2 &b, std::size_t N) {
  auto f= [&a, &b](int p, short e) {
   T ret= a(p, e) + b(p, e);
   for (int k= e; --k;) ret+= a(p, e - k) * b(p, k);
   return ret;
  };
  return multiplicative_table<T>(N, f);
 }
 // f -> g s.t. g(n) = sum_{m|n} f(m), O(N log log N)
 template <typename T> static void divisor_zeta(std::vector<T> &f) {
  std::size_t N= f.size(), i, j;
  for (sieve(N), i= 0; i < psz && ps[i] < N; i++)
   for (j= 1; ps[i] * j < N; j++) f[ps[i] * j]+= f[j];
 }
 // f -> h s.t. f(n) = sum_{m|n} h(m), O(N log log N)
 template <typename T> static void divisor_mobius(std::vector<T> &f) {
  std::size_t N= f.size(), i, j;
  for (sieve(N), i= 0; i < psz && ps[i] < N; i++)
   for (j= (N - 1) / ps[i]; j; j--) f[ps[i] * j]-= f[j];
 }
 // O(N log log N)
 template <typename T> static std::vector<T> lcm_conv(std::vector<T> a, std::vector<T> b) {
  std::size_t N= std::max(a.size(), b.size());
  a.resize(N), b.resize(N), divisor_zeta(a), divisor_zeta(b);
  for (; N--;) a[N]*= b[N];
  return divisor_mobius(a), a;
 }
 // both a and b are multiplicative, O(N)
 template <class T, class F1, class F2> static std::vector<T> lcm_conv(const F1 &a, const F2 &b, std::size_t N) {
  auto f= [&a, &b](int p, short e) {
   T suma= 1, sumb= 1;
   for (short k= 1; k < e; k++) suma+= a(p, k), sumb+= b(p, k);
   return (a(p, e) + suma) * (b(p, e) + sumb) - suma * sumb;
  };
  return multiplicative_table<T>(N, f);
 }
 // f -> g s.t. g(n) = sum_{n|m} f(m), O(N log log N)
 template <typename T> static void multiple_zeta(std::vector<T> &f) {
  std::size_t N= f.size(), i, j;
  for (sieve(N), i= 0; i < psz && ps[i] < N; i++)
   for (j= (N - 1) / ps[i]; j; j--) f[j]+= f[ps[i] * j];
 }
 // f -> h s.t. f(n) = sum_{n|m} h(m), O(N log log N)
 template <typename T> static void multiple_mobius(std::vector<T> &f) {
  std::size_t N= f.size(), i, j;
  for (sieve(N), i= 0; i < psz && ps[i] < N; i++)
   for (j= 1; ps[i] * j < N; j++) f[j]-= f[ps[i] * j];
 }
 // O(N log log N)
 template <typename T> static std::vector<T> gcd_conv(std::vector<T> a, std::vector<T> b) {
  std::size_t N= std::max(a.size(), b.size());
  a.resize(N), b.resize(N), multiple_zeta(a), multiple_zeta(b);
  for (; N--;) a[N]*= b[N];
  return multiple_mobius(a), a;
 }
};