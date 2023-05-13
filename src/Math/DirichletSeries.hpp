#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <numeric>
template <class T> struct DirichletSeries {
 using Self= DirichletSeries;
 uint64_t N;  // <= K * L
 size_t K;
 // x: i (1 <= i <= K), Xl : 1+2+...+⌊N/i⌋ (1 <= i <= L)
 std::vector<T> x, Xs, Xl;
 DirichletSeries(uint64_t N, const std::vector<T> &x, const std::vector<T> &Xs, const std::vector<T> &Xl): N(N), K(x.size() - 1), x(x), Xs(Xs), Xl(Xl) { assert(K + 1 == Xs.size()), assert(N < uint64_t(K) * Xl.size()); }
 DirichletSeries(uint64_t N, bool unit= false): N(N), K(N > 1 ? std::max(std::ceil(std::pow((double)N / std::log2(N), 2. / 3)), std::sqrt(N) + 1) : 1) {
  if (assert(N > 0), x.resize(K + 1), Xs.resize(K + 1), Xl.resize(size_t(double(N - 1 + K) / K) + 1); unit) x[1]= 1, std::fill(Xl.begin() + 1, Xl.end(), 1), std::fill(Xs.begin() + 1, Xs.end(), 1);
 }
 template <class F, std::enable_if_t<std::is_convertible_v<std::invoke_result_t<F, uint64_t>, T>, std::nullptr_t> = nullptr> DirichletSeries(uint64_t N, const F &sum): DirichletSeries(N) {
  for (size_t i= Xs.size(); --i;) Xs[i]= sum(i);
  for (size_t i= x.size(); --i;) x[i]= Xs[i] - Xs[i - 1];
  for (size_t i= Xl.size(); --i;) Xl[i]= sum(uint64_t((double)N / i));
 }
 Self &operator*=(const T &r) {
  for (size_t i= x.size(); --i;) x[i]*= r;
  for (size_t i= Xs.size(); --i;) Xs[i]*= r;
  for (size_t i= Xl.size(); --i;) Xl[i]*= r;
  return *this;
 }
 Self operator*(const T &r) const { return Self(*this)*= r; }
 Self &operator+=(const Self &r) {
  assert(N == r.N), assert(K == r.K), assert(Xl.size() == r.Xl.size());
  for (size_t i= x.size(); --i;) x[i]+= r.x[i];
  for (size_t i= Xs.size(); --i;) Xs[i]+= r.Xs[i];
  for (size_t i= Xl.size(); --i;) Xl[i]+= r.Xl[i];
  return *this;
 }
 Self &operator-=(const Self &r) {
  assert(N == r.N), assert(K == r.K), assert(Xl.size() == r.Xl.size());
  for (size_t i= x.size(); --i;) x[i]-= r.x[i];
  for (size_t i= Xs.size(); --i;) Xs[i]-= r.Xs[i];
  for (size_t i= Xl.size(); --i;) Xl[i]-= r.Xl[i];
 }
 Self operator+(const Self &r) const { return Self(*this)+= r; }
 Self operator-(const Self &r) const { return Self(*this)-= r; }
 Self operator-() const {
  std::vector<T> a(x.size()), As(Xl.size()), A(Xl.size());
  for (size_t i= x.size(); --i;) a[i]= -x[i];
  for (size_t i= Xs.size(); --i;) As[i]= -Xs[i];
  for (size_t i= Xl.size(); --i;) A[i]= -Xl[i];
  return Self(N, a, As, A);
 }
 Self &operator+=(const T &r) {
  for (size_t i= Xl.size(); --i;) Xl[i]+= r;
  for (size_t i= Xs.size(); --i;) Xs[i]+= r;
  return x[1]+= r, *this;
 }
 Self &operator-=(const T &r) {
  for (size_t i= Xl.size(); --i;) Xl[i]-= r;
  for (size_t i= Xs.size(); --i;) Xs[i]-= r;
  return x[1]-= r, *this;
 }
 Self operator+(const T &r) const { return Self(*this)+= r; }
 Self operator-(const T &r) const { return Self(*this)-= r; }
 friend Self operator+(const T &l, Self r) { return r+= l; }
 friend Self operator-(const T &l, Self r) { return -(r-= l); }
 friend Self operator*(const T &l, Self r) { return r*= l; }
 friend Self operator/(const T &l, Self r) { return (Self(r.N, true)/= r)*= l; }
 Self operator*(const Self &r) const {
  assert(N == r.N), assert(K == r.K), assert(Xl.size() == r.Xl.size());
  std::vector<T> c(K + 1), Cs(K + 1), C(Xl.size());
  uint64_t n;
  for (size_t i= K, j; i; --i)
   for (j= K / i; j; --j) c[i * j]+= x[i] * r.x[j];
  for (size_t l= Xl.size(), m, i; --l; C[l]-= sum(m) * r.sum(m))
   for (i= m= std::sqrt(n= (double)N / l); i; --i) C[l]+= x[i] * r.sum((double)n / i) + r.x[i] * sum((double)n / i);
  for (size_t i= 1; i <= K; ++i) Cs[i]= Cs[i - 1] + c[i];
  return Self(N, c, Cs, C);
 }
 Self &operator*=(const Self &r) { return *this= *this * r; }
 Self operator/(const Self &r) const { return Self(*this)/= r; }
 Self &operator/=(const Self &r) {
  assert(N == r.N), assert(K == r.K), assert(Xl.size() == r.Xl.size());
  for (size_t i= 1, j, ed; i <= K; i++)
   for (x[i]/= r.x[1], j= 2, ed= K / i; j <= ed; j++) x[i * j]-= x[i] * r.x[j];
  for (size_t i= 1; i <= K; ++i) Xs[i]= Xs[i - 1] + x[i];
  uint64_t n;
  for (size_t l= Xl.size(), m; --l; Xl[l]/= r.x[1])
   for (m= std::sqrt(n= (double)N / l), Xl[l]+= r.sum(m) * sum(m) - x[1] * r.sum(n); m > 1;) Xl[l]-= r.x[m] * sum((double)n / m) + x[m] * r.sum((double)n / m), --m;
  return *this;
 }
 Self square() const {
  std::vector<T> c(K + 1), Cs(K + 1), C(Xl.size());
  size_t i, j, l= std::sqrt(K);
  uint64_t n;
  T tmp;
  for (i= l; i; --i)
   for (j= K / i; j > i; --j) c[i * j]+= x[i] * x[j];
  for (i= K; i; --i) c[i]+= c[i];
  for (i= l; i; --i) c[i * i]+= x[i] * x[i];
  for (l= Xl.size(); --l; C[l]+= C[l], C[l]-= tmp * tmp)
   for (tmp= sum(i= std::sqrt(n= (double)N / l)); i; --i) C[l]+= x[i] * sum((double)n / i);
  for (size_t i= 1; i <= K; ++i) Cs[i]= Cs[i - 1] + c[i];
  return Self(N, c, Cs, C);
 }
 Self pow(uint64_t M) const {
  const size_t L= Xl.size() - 1;
  if (N / M > M)
   for (auto ret= Self(N, true), b= *this;; b= b.square()) {
    if (M & 1) ret*= b;
    if (!(M>>= 1)) return ret;
   }
  size_t n= 0, m, i, l, p= 2;
  uint64_t e, j;
  while (n <= M && (1ULL << n) <= N) ++n;
  Self ret(N);
  T pw[65]= {1}, b= x[1], tmp;
  for (e= M - n + 1;; b*= b)
   if (e & 1 ? pw[0]*= b : T(); !(e>>= 1)) break;
  for (m= 1; m < n; ++m) pw[m]= pw[m - 1] * x[1];
  std::vector<T> XX(Xl), z(K + 1, 0), Z(L + 1, 0);
  for (i= L; i; --i) XX[i]-= x[1];
  auto A= [&](uint64_t n) { return n <= K ? Xs[n] : XX[(double)N / n]; };
  std::vector<T> y(x), Y(XX), Bs(Xs), c(y), Cs(K + 1), C(Y);
  auto B= [&](uint64_t n) { return n <= K ? Bs[n] : Y[(double)N / n]; };
  for (tmp= pw[n - 2] * M, l= L; l; l--) C[l]*= tmp;
  for (i= 2; i <= K; ++i) c[i]*= tmp;
  for (c[1]= pw[n - 1], l= L; l; l--) C[l]+= c[1];
  for (m= 1, b= M, l= std::min<uint64_t>(L, uint64_t((double)N / p) / 2); m + 1 < n;) {
   for (b*= M - m, b/= ++m, tmp= b * pw[n - 1 - m]; l; C[l--]+= Z[l] * tmp) {
    for (i= j= std::sqrt(e= (double)N / l); i >= p; --i) Z[l]+= y[i] * A((double)e / i);
    for (i= std::min(j, e / p); i >= 2; --i) Z[l]+= x[i] * B((double)e / i);
    if (j >= p) Z[l]-= A(j) * B(j);
   }
   for (i= K; i >= p; --i)
    for (l= K / i; l >= 2; l--) z[i * l]+= y[i] * x[l];
   for (i= p= 1 << m; i <= K; ++i) c[i]+= z[i] * tmp;
   if (m + 1 == n) break;
   if (l= std::min<uint64_t>(L, uint64_t((double)N / p) / 2), y.swap(z), Y.swap(Z), std::fill_n(Z.begin() + 1, l, 0); p * 2 <= K) std::fill(z.begin() + p * 2, z.end(), 0);
   if (p <= K)
    for (Bs[p]= y[p], i= p + 1; i <= K; ++i) Bs[i]= Bs[i - 1] + y[i];
  }
  for (size_t i= 1; i <= K; ++i) Cs[i]= Cs[i - 1] + c[i];
  return Self(N, c, Cs, C);
 }
 inline T sum() const { return Xl[1]; }
 inline T sum(uint64_t n) const { return n <= K ? Xs[n] : Xl[(double)N / n]; }
};
template <class T>  // 1, zeta(s), O(N)
DirichletSeries<T> get_1(uint64_t N) {
 DirichletSeries<T> ret(N);
 for (size_t i= ret.Xl.size(); --i;) ret.Xl[i]= uint64_t((double)N / i);
 return std::fill(ret.x.begin() + 1, ret.x.end(), T(1)), std::iota(ret.Xs.begin(), ret.Xs.end(), 0), ret;
}
// Möbius, 1/zeta(s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_mu(uint64_t N) { return DirichletSeries<T>(N, true)/= get_1<T>(N); }
template <class T>  // n, zeta(s-1)
DirichletSeries<T> get_Id(uint64_t N) {
 DirichletSeries<T> ret(N);
 __uint128_t a;
 for (size_t l= ret.Xl.size(); --l;) a= (double)N / l, ret.Xl[l]= (a * (a + 1)) >> 1;
 std::iota(ret.x.begin(), ret.x.end(), 0);
 for (size_t i= 1; i <= ret.K; ++i) ret.Xs[i]= ret.Xs[i - 1] + ret.x[i];
 return ret;
}
template <class T>  // n^2, zeta(s-2), O(N)
DirichletSeries<T> get_Id2(uint64_t N) {
 DirichletSeries<T> ret(N);
 __uint128_t a, b, c;
 for (size_t l= ret.Xl.size(); --l;) a= (double)N / l, b= (a * (a + 1)) >> 1, c= (a + a + 1), ret.Xl[l]= c % 3 == 0 ? T(c / 3) * b : T(b / 3) * c;
 for (uint64_t i= ret.x.size(); --i;) ret.x[i]= i * i;
 for (size_t i= 1; i <= ret.K; ++i) ret.Xs[i]= ret.Xs[i - 1] + ret.x[i];
 return ret;
}
// number-of-divisors, zeta(s)zeta(s-1), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_d(uint64_t N) { return get_1<T>(N).square(); }
// sum-of-divisors, zeta(s)zeta(s-2), function, O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_sigma(uint64_t N) { return get_1<T>(N) * get_Id<T>(N); }
// Euler's totient, zeta(s-1)/zeta(s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_phi(uint64_t N) { return get_Id<T>(N)/= get_1<T>(N); }
template <class T>  // zeta(2s), O(N)
DirichletSeries<T> get_1sq(uint64_t N) {
 DirichletSeries<T> ret(N);
 for (size_t i= 1, e= ret.x.size(); i * i <= e; ++i) ret.x[i * i]= 1;
 for (size_t i= 1; i <= ret.K; ++i) ret.Xs[i]= ret.Xs[i - 1] + ret.x[i];
 for (size_t l= ret.Xl.size(); --l;) ret.Xl[l]= uint64_t(std::sqrt((double)N / l));
 return ret;
}
// Liouville, zeta(2s)/zeta(s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_lambda(uint64_t N) { return get_1sq<T>(N)/= get_1<T>(N); }
// |µ|, zeta(s)/zeta(2s), O(N^(2/3)log^(1/3)N))
template <class T> DirichletSeries<T> get_absmu(uint64_t N) { return get_1<T>(N)/= get_1sq<T>(N); }