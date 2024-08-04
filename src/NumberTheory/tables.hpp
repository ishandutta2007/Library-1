#pragma once
#include "src/NumberTheory/enumerate_primes.hpp"
namespace nt_internal {
vector<int> lfw;
vector<char> lfe;
void set_lfe(int N) {
 static int n= 3, i= 1;
 if (n > N) return;
 for (sieve(N), lfw.resize((N + 1) >> 1), copy(lf.begin() + i, lf.begin() + ((N + 1) >> 1), lfw.begin() + i), lfe.resize(((N + 1) >> 1), 1); n <= N; n+= 2, ++i)
  if (int j= (n / lf[i]) >> 1; lf[i] == lf[j]) lfe[i]+= lfe[j], lfw[i]*= lfw[j];
}
// O(log n)
vector<pair<int, short>> factorize(int n) {
 vector<pair<int, short>> ret;
 if (short t; !(n & 1)) ret.emplace_back(2, t= __builtin_ctz(n)), n>>= t;
 if (int i= n >> 1; n > 1)
  for (set_lfe(n); n > 1; i= n >> 1) ret.emplace_back(lf[i], lfe[i]), n/= lfw[i];
 return ret;
}
// f(p,e) := f(p^e)
template <class T, class F> vector<T> multiplicative_table(int N, const F &f) {
 vector<T> ret(N + 1);
 set_lfe(N);
 for (int n= 3, i= 1; n <= N; n+= 2, ++i) ret[n]= lfw[i] == n ? f(lf[i], lfe[i]) : ret[lfw[i]] * ret[n / lfw[i]];
 for (int n= 2, t; n <= N; n+= 2) t= __builtin_ctz(n), ret[n]= n & (n - 1) ? ret[n & -n] * ret[n >> t] : f(2, t);
 return ret[1]= 1, ret;
}
// O(N)
template <class T= int> vector<T> mobius_table(int N) {
 vector<T> ret(N + 1);
 set_lfe(N), ret[1]= 1;
 for (int n= 3, i= 1; n <= N; n+= 2, ++i) ret[n]= lfw[i] == n ? -(lfe[i] == 1) : ret[lfw[i]] * ret[n / lfw[i]];
 for (int n= 2; n <= N; n+= 4) ret[n]= -ret[n >> 1];
 return ret;
}
// O(N)
template <class T= int> vector<T> totient_table(int N) {
 vector<T> ret(N + 1);
 set_lfe(N), ret[1]= 1;
 for (int n= 3, i= 1; n <= N; n+= 2, ++i) ret[n]= lfw[i] == n ? lf[i] == n ? n - 1 : ret[n / lf[i]] * lf[i] : ret[lfw[i]] * ret[n / lfw[i]];
 for (int n= 2; n <= N; n+= 4) ret[n]= ret[n >> 1];
 for (int n= 4; n <= N; n+= 4) ret[n]= ret[n >> 1] << 1;
 return ret;
}
}
using nt_internal::factorize, nt_internal::multiplicative_table, nt_internal::mobius_table, nt_internal::totient_table;
// f -> g s.t. g(n) = sum_{m|n} f(m), O(N log log N)
template <typename T> void divisor_zeta(std::vector<T> &a) {
 for (auto p: enumerate_primes(a.size() - 1))
  for (int j= 1, e= a.size(); p * j < e; ++j) a[p * j]+= a[j];
}
// a -> h s.t. a(n) = sum_{m|n} h(m), O(N log log N)
template <typename T> void divisor_mobius(std::vector<T> &a) {
 for (auto p: enumerate_primes(a.size() - 1))
  for (int j= (a.size() - 1) / p; j; --j) a[p * j]-= a[j];
}
// O(N log log N)
template <typename T> std::vector<T> lcm_convolve(std::vector<T> a, std::vector<T> b) {
 std::size_t N= std::max(a.size(), b.size());
 for (a.resize(N), b.resize(N), divisor_zeta(a), divisor_zeta(b); N--;) a[N]*= b[N];
 return divisor_mobius(a), a;
}
// a -> g s.t. g(n) = sum_{n|m} a(m), O(N log log N)
template <typename T> static void multiple_zeta(std::vector<T> &a) {
 for (auto p: enumerate_primes(a.size() - 1))
  for (int j= (a.size() - 1) / p; j; --j) a[j]+= a[p * j];
}
// a -> h s.t. a(n) = sum_{n|m} h(m), O(N log log N)
template <typename T> static void multiple_mobius(std::vector<T> &a) {
 for (auto p: enumerate_primes(a.size() - 1))
  for (int j= 1, e= a.size(); p * j < e; ++j) a[j]-= a[p * j];
}
// O(N log log N)
template <typename T> static std::vector<T> gcd_convolve(std::vector<T> a, std::vector<T> b) {
 std::size_t N= std::max(a.size(), b.size());
 for (a.resize(N), b.resize(N), multiple_zeta(a), multiple_zeta(b); N--;) a[N]*= b[N];
 return multiple_mobius(a), a;
}