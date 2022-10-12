#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_inv.hpp"
#include "src/FFT/convolve.hpp"

/**
 * @title 線形漸化的数列の第$k$項
 * @category FFT
 * https://qiita.com/ryuhe1/items/da5acbcce4ac1911f47a
 */

// BEGIN CUT HERE
namespace div_at_internal {
template <class K>
int deg(const std::vector<K> &p) {
  const K ZERO = 0;
  for (int n = p.size() - 1;; n--)
    if (n < 0 || p[n] != ZERO) return n;
}
template <class mod_t>
void div_at_na(std::vector<mod_t> &p, std::vector<mod_t> &q, std::uint64_t &k) {
  unsigned n = deg(p), nn, j;
  const unsigned m = deg(q), l = std::max(n, m) + 1;
  const mod_t Z = 0;
  std::vector<mod_t> tmp(l);
  for (p.resize(l, Z), q.resize(l, Z); k > m; q.swap(p), p.swap(tmp)) {
    std::fill_n(tmp.begin(), (nn = (n + m - ((n ^ m ^ k) & 1)) >> 1) + 1, Z);
    for (j = 0; j <= m; j += 2)
      for (int i = k & 1; i <= n; i += 2) tmp[(i + j) >> 1] += p[i] * q[j];
    for (j = 1; j <= m; j += 2)
      for (int i = (~k) & 1; i <= n; i += 2) tmp[(i + j) >> 1] -= p[i] * q[j];
    for (std::fill_n(p.begin(), m + 1, Z), j = 2; j <= m; j += 2)
      for (int i = j; (i -= 2) >= 0;) p[(i + j) >> 1] += q[i] * q[j];
    for (k >>= 1, n = nn, j = 3; j <= m; j += 2)
      for (int i = j; (i -= 2) >= 0;) p[(i + j) >> 1] -= q[i] * q[j];
    for (int i = m; i >= 0; i--) p[i] += p[i];
    for (int i = 0; i <= m; i += 2) p[i] += q[i] * q[i];
    for (int i = 1; i <= m; i += 2) p[i] -= q[i] * q[i];
  }
  p.resize(n + 1), q.resize(m + 1);
}
template <std::size_t _Nm, class mod_t>
void div_at_ntt(std::vector<mod_t> &p, std::vector<mod_t> &q,
                std::uint64_t &k) {
  static_assert(!is_nttfriend<mod_t, _Nm>());
  using GNA = GlobalNTTArray<mod_t, _Nm, 0>;
  using GNA1 = GlobalNTTArray<mod_t, _Nm, 1>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm, 2>;
  const unsigned m = deg(q) + 1, offset = std::max<unsigned>(deg(p) + 1, m),
                 len = get_len((offset + m) - 1);
  for (p.resize(len >> 1, mod_t(0)); k >= offset; k >>= 1) {
    GNA::bf.set(p.data(), 0, len >> 1), GNA::bf.zeros(len >> 1, len);
    GNA1::bf.set(q.data(), 0, m), GNA1::bf.zeros(m, len);
    GNA2::bf.zeros(m, len);
    for (int i = m; i--;) GNA2::bf.set(i, i & 1 ? -q[i] : q[i]);
    GNA::bf.dft(0, len), GNA1::bf.dft(0, len), GNA2::bf.dft(0, len);
    GNA::bf.mul(GNA2::bf, 0, len), GNA::bf.idft(0, len);
    GNA1::bf.mul(GNA2::bf, 0, len), GNA1::bf.idft(0, len);
    for (int i = k & 1; i < len; i += 2) p[i >> 1] = GNA::bf.get(i);
    for (int i = m; i--;) q[i] = GNA1::bf.get(i << 1);
  }
}
template <std::size_t _Nm, class mod_t>
void div_at_ntt_fast(std::vector<mod_t> &p, std::vector<mod_t> &q,
                     std::uint64_t &k) {
  static_assert(is_nttfriend<mod_t, _Nm>());
  using NTT = NumberTheoreticTransform<mod_t>;
  const unsigned m = deg(q) + 1, offset = std::max<unsigned>(deg(p) + 1, m),
                 len = get_len((offset + m) - 1), len2 = len >> 1;
  p.resize(len, mod_t(0)), q.resize(len, mod_t(0));
  NTT::dft(len, p.data()), NTT::dft(len, q.data());
  while (1) {
    for (int i = len; i--;) p[i] *= q[i ^ 1];
    k & 1 ? NTT::odd_dft(len, p.data()) : NTT::even_dft(len, p.data());
    for (int i = 0; i < len; i += 2) q[i] = q[i + 1] = q[i] * q[i + 1];
    NTT::even_dft(len, q.data());
    if ((k >>= 1) < offset) break;
    NTT::dft_doubling(len2, p.data()), NTT::dft_doubling(len2, q.data());
  }
  NTT::idft(len2, p.data()), NTT::idft(len2, q.data());
}
}  // namespace div_at_internal
#define __FPS_DIVAT(Vec)                                                \
  template <class mod_t, std::size_t _Nm = 1 << 22>                     \
  mod_t div_at(Vec p, Vec q, std::uint64_t k) {                         \
    using namespace div_at_internal;                                    \
    const int n = deg(p) + 1, m = deg(q) + 1;                           \
    assert(m != 0);                                                     \
    mod_t ret = 0;                                                      \
    if (n == 0) return ret;                                             \
    if (m == 1) return k <= (std::uint64_t)n ? p[k] / q[0] : ret;       \
    if (k >= m) {                                                       \
      if constexpr (is_nttfriend<mod_t, _Nm>())                         \
        m <= 44 ? div_at_na(p, q, k) : div_at_ntt_fast<_Nm>(p, q, k);   \
      else                                                              \
        m <= 340 ? div_at_na(p, q, k) : div_at_ntt<_Nm>(p, q, k);       \
    }                                                                   \
    p.resize(k + 1, ret), q.resize(k + 1, ret), q = inv<mod_t, _Nm>(q); \
    for (int i = k; i >= 0; i--) ret += q[i] * p[k - i];                \
    return ret;                                                         \
  }

__FPS_DIVAT(std::vector<mod_t>)
#ifdef __POLYNOMIAL
__FPS_DIVAT(__POLYNOMIAL)
#endif

// a[n] = c[0] * a[n-1] + c[1] * a[n-2] + ... + c[d-1] * a[n-d]
// return a[k]
template <class mod_t, std::size_t _Nm = 1 << 22>
mod_t linear_recurrence(std::vector<mod_t> c, std::vector<mod_t> a,
                        std::uint64_t k) {
  const std::size_t d = c.size();
  assert(d <= a.size());
  for (auto &x : c) x = -x;
  c.insert(c.begin(), mod_t(1)), a.resize(d);
  auto p = convolve<mod_t, _Nm>(c, a);
  return p.resize(d), div_at<mod_t, _Nm>(p, c, k);
}
