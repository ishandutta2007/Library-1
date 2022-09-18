#pragma once
#include <bits/stdc++.h>
/**
 * @title 数論関数の累積和
 * @category 数学
 * @see
 * https://maspypy.com/dirichlet-%E7%A9%8D%E3%81%A8%E3%80%81%E6%95%B0%E8%AB%96%E9%96%A2%E6%95%B0%E3%81%AE%E7%B4%AF%E7%A9%8D%E5%92%8C
 * O(KlogK + √(NL))
 */

// verify用:
// https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d

// BEGIN CUT HERE

template <class T>
struct DirichletConvSumTable {
  std::uint64_t N;  // <= K * L
  std::vector<T> x /* (1 <= i <= K) */, X /* ∑^{N/i} (1 <= i <= L) */;
  static DirichletConvSumTable get_epsilon(std::uint64_t N, std::size_t K) {
    std::size_t L = (N - 1 + K) / K;
    std::vector<T> a(K + 1, 0);
    return a[1] = 1, DirichletConvSumTable(N, a, std::vector<T>(L + 1, 1));
  }
  DirichletConvSumTable(std::uint64_t n_, const std::vector<T> &x_,
                        const std::vector<T> &X_)
      : N(n_), x(x_), X(X_) {
    assert(N < std::uint64_t(x.size()) * X.size());
  }
  DirichletConvSumTable(std::uint64_t n_, std::size_t k_)
      : N(n_), x(k_ + 1, 0), X((n_ - 1 + k_) / k_ + 1, 0) {}
  template <class F>
  DirichletConvSumTable(std::uint64_t n_, std::size_t k_, const F &sum)
      : N(n_), x(k_ + 1), X((n_ - 1 + k_) / k_ + 1) {
    assert(N < std::uint64_t(x.size()) * X.size());
    for (std::size_t i = x.size(); --i;) x[i] = sum(i);
    for (std::size_t i = x.size(); --i > 1;) x[i] -= x[i - 1];
    for (std::size_t i = X.size(); --i;) X[i] = sum(N / i);
  }
  DirichletConvSumTable operator*(const DirichletConvSumTable &r) const {
    const std::size_t K = x.size() - 1, L = X.size() - 1;
    assert(N <= std::uint64_t(K) * L), assert(N == r.N);
    assert(K == r.x.size() - 1), assert(L == r.X.size() - 1);
    std::vector<T> c(K + 1, 0), C(L + 1, 0), A_l(K + 1, 0), B_l(K + 1, 0);
    for (int i = 1; i <= K; i++) A_l[i] = A_l[i - 1] + x[i];
    for (int i = 1; i <= K; i++) B_l[i] = B_l[i - 1] + r.x[i];
    auto A = [&](std::uint64_t n) { return n <= K ? A_l[n] : X[N / n]; };
    auto B = [&](std::uint64_t n) { return n <= K ? B_l[n] : r.X[N / n]; };
    std::uint64_t n;
    for (std::size_t i = K, j; i; i--)
      for (j = K / i; j; j--) c[i * j] += x[i] * r.x[j];
    for (std::size_t l = L, m, i; l; C[l--] -= A(m) * B(m))
      for (i = m = std::sqrt(n = N / l); i; i--)
        C[l] += x[i] * B(n / i) + r.x[i] * A(n / i);
    return DirichletConvSumTable<T>(N, c, C);
  }
  DirichletConvSumTable &operator*=(const DirichletConvSumTable &r) {
    return *this = *this * r;
  }
  DirichletConvSumTable operator/(const DirichletConvSumTable &r) const {
    return DirichletConvSumTable(*this) /= r;
  }
  DirichletConvSumTable &operator/=(const DirichletConvSumTable &r) {
    const std::size_t K = x.size() - 1, L = X.size() - 1;
    assert(N <= std::uint64_t(K) * L), assert(N == r.N);
    assert(K == r.x.size() - 1), assert(L == r.X.size() - 1);
    std::vector<T> A_l(K + 1, 0), B_l(K + 1, 0);
    for (std::size_t i = 1, j, ed; i <= K; i++)
      for (x[i] /= r.x[1], j = 2, ed = K / i; j <= ed; j++)
        x[i * j] -= x[i] * r.x[j];
    for (std::size_t i = 1; i <= K; i++) A_l[i] = A_l[i - 1] + r.x[i];
    for (std::size_t i = 1; i <= K; i++) B_l[i] = B_l[i - 1] + x[i];
    auto A = [&](std::uint64_t n) { return n <= K ? A_l[n] : r.X[N / n]; };
    auto B = [&](std::uint64_t n) { return n <= K ? B_l[n] : X[N / n]; };
    std::uint64_t n;
    for (std::size_t l = L, m; l; X[l--] /= r.x[1])
      for (X[l] += A(m = std::sqrt(n = N / l)) * B(m) - x[1] * A(n); m > 1; m--)
        X[l] -= r.x[m] * B(n / m) + x[m] * A(n / m);
    return *this;
  }
  DirichletConvSumTable square() const {
    const std::size_t K = x.size() - 1, L = X.size() - 1;
    assert(N <= std::uint64_t(K) * L);
    std::vector<T> c(K + 1, 0), C(L + 1, 0), A_l(K + 1, 0);
    for (int i = 1; i <= K; i++) A_l[i] = A_l[i - 1] + x[i];
    auto A = [&](std::uint64_t n) { return n <= K ? A_l[n] : X[N / n]; };
    std::size_t i, j, l = std::sqrt(K);
    std::uint64_t n;
    T tmp;
    for (i = l; i; i--)
      for (j = K / i; j > i; j--) c[i * j] += x[i] * x[j];
    for (i = K; i; i--) c[i] += c[i];
    for (i = l; i; i--) c[i * i] += x[i] * x[i];
    for (l = L; l; C[l] += C[l], C[l--] -= tmp * tmp)
      for (tmp = A(i = std::sqrt(n = N / l)); i; i--) C[l] += x[i] * A(n / i);
    return DirichletConvSumTable<T>(N, c, C);
  }
  DirichletConvSumTable pow1(std::uint64_t M) const {
    for (auto ret = get_epsilon(N, x.size() - 1), b = *this;; b = b.square()) {
      if (M & 1) ret *= b;
      if (!(M >>= 1)) return ret;
    }
  }
  DirichletConvSumTable pow2(std::uint64_t M) const {
    const std::size_t K = x.size() - 1, L = X.size() - 1;
    assert(N <= std::uint64_t(K) * L);
    if (M == 0) return get_epsilon(N, K);
    if (M == 1) return *this;
    std::size_t n = 0, m, i, l, p = 2;
    std::uint64_t e, j;
    while (n <= M && (1ULL << n) <= N) n++;
    DirichletConvSumTable ret(N, x.size() - 1);
    T pw[65] = {1}, b = x[1], tmp;
    for (e = M - n + 1;; b *= b)
      if (e & 1 ? pw[0] *= b : 0; !(e >>= 1)) break;
    for (m = 1; m < n; m++) pw[m] = pw[m - 1] * x[1];
    std::vector<T> XX(X), z(K + 1, 0), Z(L + 1, 0), A_l(K + 1, 0);
    for (i = 2; i <= K; i++) A_l[i] = A_l[i - 1] + x[i];
    for (i = L; i; i--) XX[i] -= x[1];
    auto A = [&](std::uint64_t n) { return n <= K ? A_l[n] : XX[N / n]; };
    std::vector<T> y(x), Y(XX), B_l(A_l), c(y), C(Y);
    auto B = [&](std::uint64_t n) { return n <= K ? B_l[n] : Y[N / n]; };
    for (tmp = pw[n - 2] * M, l = L; l; l--) C[l] *= tmp;
    for (i = 2; i <= K; i++) c[i] *= tmp;
    for (c[1] = pw[n - 1], l = L; l; l--) C[l] += c[1];
    for (m = 1, b = M, l = std::min<std::uint64_t>(L, N / p / 2); m + 1 < n;) {
      b *= M - m, b /= ++m, tmp = b * pw[n - 1 - m];
      for (; l; C[l--] += Z[l] * tmp) {
        for (i = j = std::sqrt(e = N / l); i >= p; i--) Z[l] += y[i] * A(e / i);
        for (i = std::min(j, e / p); i >= 2; i--) Z[l] += x[i] * B(e / i);
        if (j >= p) Z[l] -= A(j) * B(j);
      }
      for (i = K; i >= p; i--)
        for (l = K / i; l >= 2; l--) z[i * l] += y[i] * x[l];
      for (i = p = 1 << m; i <= K; i++) c[i] += z[i] * tmp;
      if (m + 1 == n) break;
      l = std::min<std::uint64_t>(L, N / p / 2);
      y.swap(z), Y.swap(Z), std::fill_n(Z.begin() + 1, l, 0);
      if (p * 2 <= K) std::fill(z.begin() + p * 2, z.end(), 0);
      if (p <= K)
        for (B_l[p] = y[p], i = p + 1; i <= K; i++) B_l[i] = B_l[i - 1] + y[i];
    }
    return DirichletConvSumTable<T>(N, c, C);
  }
  inline T sum() const { return X[1]; }
};

template <class T>  // zeta(s)
DirichletConvSumTable<T> get_1(std::uint64_t N, std::size_t K) {
  std::size_t L = (N - 1 + K) / K;
  std::vector<T> A(L + 1);
  for (std::size_t l = L; l; l--) A[l] = N / l;
  return DirichletConvSumTable<T>(N, std::vector<T>(K + 1, 1), A);
}

template <class T>  // Möbius, O(KlogK + √(NL))
DirichletConvSumTable<T> get_mu(std::uint64_t N, std::size_t K) {
  return DirichletConvSumTable<T>::get_epsilon(N, K) / get_1<T>(N, K);
}

template <class T>  // zeta(s-1)
DirichletConvSumTable<T> get_Id(std::uint64_t N, std::size_t K) {
  std::size_t L = (N - 1 + K) / K;
  std::vector<T> a(K + 1), A(L + 1);
  for (std::size_t l = L; l; l--) A[l] = N / l, (A[l] *= A[l] + 1) /= 2;
  return std::iota(a.begin(), a.end(), 0), DirichletConvSumTable<T>(N, a, A);
}

template <class T>  // Euler's totient, O(KlogK + √(NL))
DirichletConvSumTable<T> get_phi(std::uint64_t N, std::size_t K) {
  return get_Id<T>(N, K) / get_1<T>(N, K);
}

template <class T>  // zeta(2s)
DirichletConvSumTable<T> get_1sq(std::uint64_t N, std::size_t K) {
  std::size_t L = (N - 1 + K) / K;
  std::vector<T> a(K + 1, 0), A(L + 1);
  for (std::size_t i = 1; i * i <= K; i++) a[i * i] = 1;
  for (std::size_t l = L; l; l--) A[l] = sqrt(N / l);
  return DirichletConvSumTable<T>(N, a, A);
}

template <class T>  // Liouville, O(KlogK + √(NL))
DirichletConvSumTable<T> get_lambda(std::uint64_t N, std::size_t K) {
  return get_1sq<T>(N, K) / get_1<T>(N, K);
}

template <class T>  // |µ|, O(KlogK + √(NL))
DirichletConvSumTable<T> get_absmu(std::uint64_t N, std::size_t K) {
  return get_1<T>(N, K) / get_1sq<T>(N, K);
}

template <class T>  // O(√N)
T dirichlet_mul_sum(const DirichletConvSumTable<T> &a,
                    const DirichletConvSumTable<T> &b) {
  const std::size_t K = a.x.size() - 1, L = a.X.size() - 1, M = std::min(K, L);
  assert(a.N == b.N), assert(M <= b.x.size() - 1), assert(M <= b.X.size() - 1);
  assert(std::uint64_t(M + 1) * (M + 1) > a.N);
  T ret = 0, A = 0, B = 0;
  for (int i = M; i; i--) ret += a.x[i] * b.X[i] + b.x[i] * a.X[i];
  for (int i = M; i; i--) A += a.x[i], B += b.x[i];
  return ret -= A * B;
}