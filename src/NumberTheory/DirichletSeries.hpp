#pragma once
#include <valarray>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <numeric>
#include <cstdint>
template <class T> struct DirichletSeries {
 using Self= DirichletSeries;
 uint64_t N;  // <= K * L
 size_t K, L;
 std::valarray<T> x, X;
 DirichletSeries(uint64_t N, bool unit= false): N(N), K(N > 1 ? std::max(std::ceil(std::pow((double)N / std::log2(N), 2. / 3)), std::sqrt(N) + 1) : 1), L((N - 1 + K) / K), x(K + 1), X(K + L + 1) {
  if (assert(N > 0); unit) x[1]= 1, X= 1;
 }
 template <class F, typename= std::enable_if_t<std::is_convertible_v<std::invoke_result_t<F, uint64_t>, T>>> DirichletSeries(uint64_t N, const F &sum): DirichletSeries(N) {
  for (size_t i= 1; i <= K; ++i) X[i]= sum(i);
  for (size_t i= 1; i <= L; ++i) X[K + i]= sum(uint64_t((double)N / i));
  for (size_t i= K; i; --i) x[i]= X[i] - X[i - 1];
 }
 Self operator-() const {
  Self ret(N);
  return ret.x= -x, ret.X= -X, ret;
 }
 Self &operator+=(T r) { return x[1]+= r, X+= r, *this; }
 Self &operator-=(T r) { return x[1]-= r, X-= r, *this; }
 Self &operator*=(T r) { return x*= r, X*= r, *this; }
 Self &operator/=(T r) {
  if (T iv= T(1) / r; iv == 0) x/= r, X/= r;
  else x*= iv, X*= iv;
  return *this;
 }
 Self &operator+=(const Self &r) { return assert(N == r.N), assert(K == r.K), assert(L == r.L), x+= r.x, X+= r.X, *this; }
 Self &operator-=(const Self &r) { return assert(N == r.N), assert(K == r.K), assert(L == r.L), x-= r.x, X-= r.X, *this; }
 Self operator+(T r) const { return Self(*this)+= r; }
 Self operator-(T r) const { return Self(*this)-= r; }
 Self operator*(T r) const { return Self(*this)*= r; }
 Self operator/(T r) const { return Self(*this)/= r; }
 Self operator+(const Self &r) const { return Self(*this)+= r; }
 Self operator-(const Self &r) const { return Self(*this)-= r; }
 friend Self operator+(T l, Self r) { return r+= l; }
 friend Self operator-(T l, Self r) { return r.x[1]-= l, r.X-= l, r.x= -r.x, r.X= -r.X, r; }
 friend Self operator*(T l, const Self &r) { return r * l; }
 friend Self operator/(T l, const Self &r) { return (Self(r.N, true)/= r)*= l; }
 Self operator*(const Self &r) const {
  assert(N == r.N), assert(K == r.K), assert(L == r.L);
  Self ret(N);
  uint64_t n;
  for (size_t i= K, j; i; --i)
   for (j= K / i; j; --j) ret.x[i * j]+= x[i] * r.x[j];
  for (size_t l= L, m, i; l; ret.X[K + l--]-= sum(m) * r.sum(m))
   for (i= m= std::sqrt(n= (double)N / l); i; --i) ret.X[K + l]+= x[i] * r.sum((double)n / i) + r.x[i] * sum((double)n / i);
  for (size_t i= 1; i <= K; ++i) ret.X[i]= ret.X[i - 1] + ret.x[i];
  return ret;
 }
 Self operator/(const Self &r) const { return Self(*this)/= r; }
 Self &operator*=(const Self &r) { return *this= *this * r; }
 Self &operator/=(const Self &r) {
  assert(N == r.N), assert(K == r.K), assert(L == r.L);
  for (size_t i= 1, j, ed; i <= K; i++)
   for (x[i]/= r.x[1], j= 2, ed= K / i; j <= ed; j++) x[i * j]-= x[i] * r.x[j];
  X[1]= x[1];
  for (size_t i= 2; i <= K; ++i) X[i]= X[i - 1] + x[i];
  uint64_t n;
  for (size_t l= L, m; l; X[K + l--]/= r.x[1])
   for (m= std::sqrt(n= (double)N / l), X[K + l]+= r.sum(m) * sum(m) - x[1] * r.sum(n); m > 1;) X[K + l]-= r.x[m] * sum((double)n / m) + x[m] * r.sum((double)n / m), --m;
  return *this;
 }
 Self square() const {
  Self ret(N);
  size_t i, j, l= std::sqrt(K);
  uint64_t n;
  T tmp;
  for (i= l; i; --i)
   for (j= K / i; j > i; --j) ret.x[i * j]+= x[i] * x[j];
  ret.x+= ret.x;
  for (i= l; i; --i) ret.x[i * i]+= x[i] * x[i];
  for (l= L; l; ret.X[K + l]+= ret.X[K + l], ret.X[K + l--]-= tmp * tmp)
   for (tmp= sum(i= std::sqrt(n= (double)N / l)); i; --i) ret.X[K + l]+= x[i] * sum((double)n / i);
  for (size_t i= 1; i <= K; ++i) ret.X[i]= ret.X[i - 1] + ret.x[i];
  return ret;
 }
 Self pow(uint64_t M) const {
  if (N / M > M)
   for (auto ret= Self(N, true), b= *this;; b= b.square()) {
    if (M & 1) ret*= b;
    if (!(M>>= 1)) return ret;
   }
  size_t n= 0, m, i, l, p= 2;
  uint64_t e, j;
  while (n <= M && (1ULL << n) <= N) ++n;
  T pw[65]= {1}, b= x[1], tmp;
  for (e= M - n + 1;; b*= b)
   if (e & 1 ? pw[0]*= b : T(); !(e>>= 1)) break;
  for (m= 1; m < n; ++m) pw[m]= pw[m - 1] * x[1];
  Self ret(*this);
  std::valarray<T> D= (ret.X-= x[1]), E(std::begin(D), K + 1), Y(std::begin(D) + K, L + 1), y= x, z(K + 1), Z(L + 1);
  auto A= [&](uint64_t n) { return n > K ? D[K + (double)N / n] : D[n]; };
  auto B= [&](uint64_t n) { return n > K ? Y[(double)N / n] : E[n]; };
  for (tmp= pw[n - 2] * M, l= L; l; l--) ret.X[K + l]*= tmp;
  for (i= 2; i <= K; ++i) ret.x[i]*= tmp;
  for (ret.x[1]= pw[n - 1], l= L; l; l--) ret.X[K + l]+= ret.x[1];
  for (m= 1, b= M, l= std::min<uint64_t>(L, uint64_t((double)N / p) / 2); m + 1 < n;) {
   for (b*= M - m, b/= ++m, tmp= b * pw[n - 1 - m]; l; ret.X[K + l--]+= Z[l] * tmp) {
    for (i= j= std::sqrt(e= (double)N / l); i >= p; --i) Z[l]+= y[i] * A((double)e / i);
    for (i= std::min(j, e / p); i >= 2; --i) Z[l]+= x[i] * B((double)e / i);
    if (j >= p) Z[l]-= A(j) * B(j);
   }
   for (i= K; i >= p; --i)
    for (l= K / i; l >= 2; l--) z[i * l]+= y[i] * x[l];
   for (i= p= 1 << m; i <= K; ++i) ret.x[i]+= z[i] * tmp;
   if (m + 1 == n) break;
   if (l= std::min<uint64_t>(L, uint64_t((double)N / p) / 2), y.swap(z), Y.swap(Z), std::fill_n(std::begin(Z) + 1, l, 0); p * 2 <= K) std::fill(std::begin(z) + p * 2, std::end(z), 0);
   if (p <= K)
    for (E[p]= y[p], i= p + 1; i <= K; ++i) E[i]= E[i - 1] + y[i];
  }
  for (size_t i= 1; i <= K; ++i) ret.X[i]= ret.X[i - 1] + ret.x[i];
  return ret;
 }
 inline T sum() const { return X[K + 1]; }
 inline T sum(uint64_t n) const { return n > K ? X[K + (double)N / n] : X[n]; }
 inline T operator()(uint64_t n) const { return n > K ? x[K + (double)N / n] : x[n]; }
};
// 1, zeta(s), O(K+L)
template <class T> DirichletSeries<T> get_1(uint64_t N) {
 DirichletSeries<T> ret(N);
 for (size_t i= ret.L; i; --i) ret.X[ret.K + i]= uint64_t((double)N / i);
 return std::fill(std::begin(ret.x) + 1, std::end(ret.x), T(1)), std::iota(std::begin(ret.X), std::begin(ret.X) + ret.K + 1, 0), ret;
}
// Mobius, 1/zeta(s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_mu(uint64_t N) { return DirichletSeries<T>(N, true)/= get_1<T>(N); }
// n, zeta(s-1)
template <class T> DirichletSeries<T> get_Id(uint64_t N) {
 DirichletSeries<T> ret(N);
 __uint128_t a;
 for (size_t l= ret.L; l; --l) a= (double)N / l, ret.X[ret.K + l]= (a * (a + 1)) >> 1;
 std::iota(std::begin(ret.x), std::end(ret.x), 0);
 for (size_t i= 1; i <= ret.K; ++i) ret.X[i]= ret.X[i - 1] + ret.x[i];
 return ret;
}
// n^2, zeta(s-2), O(K+L)
template <class T> DirichletSeries<T> get_Id2(uint64_t N) {
 DirichletSeries<T> ret(N);
 __uint128_t a, b, c;
 for (size_t l= ret.L; l; --l) a= (double)N / l, b= (a * (a + 1)) >> 1, c= (a + a + 1), ret.X[ret.K + l]= c % 3 == 0 ? T(c / 3) * b : T(b / 3) * c;
 for (uint64_t i= ret.K; i; --i) ret.x[i]= i * i;
 for (size_t i= 1; i <= ret.K; ++i) ret.X[i]= ret.X[i - 1] + ret.x[i];
 return ret;
}
// number-of-divisors, zeta(s)zeta(s-1), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_d(uint64_t N) { return get_1<T>(N).square(); }
// sum-of-divisors, zeta(s)zeta(s-2), function, O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_sigma(uint64_t N) { return get_1<T>(N) * get_Id<T>(N); }
// Euler's totient, zeta(s-1)/zeta(s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_phi(uint64_t N) { return get_Id<T>(N)/= get_1<T>(N); }
template <class T>  // zeta(2s), O(K+L)
DirichletSeries<T> get_1sq(uint64_t N) {
 DirichletSeries<T> ret(N);
 for (size_t i= 1, e= ret.x.size(); i * i <= e; ++i) ret.x[i * i]= 1;
 for (size_t i= 1; i <= ret.K; ++i) ret.X[i]= ret.X[i - 1] + ret.x[i];
 for (size_t l= ret.L; l; --l) ret.X[ret.K + l]= uint64_t(std::sqrt((double)N / l));
 return ret;
}
// Liouville, zeta(2s)/zeta(s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_lambda(uint64_t N) { return get_1sq<T>(N)/= get_1<T>(N); }
// square-free, zeta(s)/zeta(2s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_absmu(uint64_t N) { return get_1<T>(N)/= get_1sq<T>(N); }
