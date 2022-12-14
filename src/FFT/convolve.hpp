#pragma once
#include <bits/stdc++.h>

#include "src/FFT/NTT.hpp"

/**
 * @title 畳み込み
 * @category FFT
 */

// BEGIN CUT HERE
template <class mod_t, std::size_t LIM = 1 << 22>
std::vector<mod_t> convolve(const std::vector<mod_t> &p,
                            const std::vector<mod_t> &q) {
  using GAr = GlobalArray<mod_t, LIM, 0>;
  using GAp = GlobalArray<mod_t, LIM, 1>;
  using GAq = GlobalArray<mod_t, LIM, 2>;
  static constexpr int t = nttarr_cat<mod_t, LIM>;
  auto f = [](int l) -> int {
    static constexpr double B[] = {
        (double[]){8.288, 5.418, 7.070, 9.676, 11.713, 13.374}[t],
        (double[]){8.252, 6.578, 9.283, 12.810, 13.853, 15.501}[t]};
    return std::round(std::pow(l, 0.535) * B[__builtin_ctz(l) & 1]);
  };
  const int n = p.size(), m = q.size(), sz = n + m - 1;
  if (!n || !m) return std::vector<mod_t>();
  if (std::min(n, m) < (int[]){70, 30, 70, 100, 135, 150}[t]) {
    std::fill_n(GAr::bf, sz, mod_t(0));
    std::copy(p.begin(), p.end(), GAp::bf);
    std::copy(q.begin(), q.end(), GAq::bf);
    for (int i = n; i--;)
      for (int j = m; j--;) GAr::bf[i + j] += GAp::bf[i] * GAq::bf[j];
  } else {
    const int rl = get_len(sz), l = get_len(std::max(n, m)), fl = f(l);
    static constexpr std::size_t LIM2 = LIM >> 4;
    static constexpr bool b = nttarr_cat<mod_t, LIM2> < t;
    if (b || (l + fl < sz && sz <= (rl >> 3) * 5)) {
      using GNA1 = GlobalNTTArray<mod_t, LIM2, 1>;
      using GNA2 = GlobalNTTArray<mod_t, LIM2, 2>;
      using GNA2D1 = GlobalNTTArray2D<mod_t, LIM2, 16, 1>;
      using GNA2D2 = GlobalNTTArray2D<mod_t, LIM2, 16, 2>;
      const int l = rl >> 4, l2 = l << 1;
      const int nn = (n + l - 1) / l, mm = (m + l - 1) / l;
      for (int i = 0, k = 0, s; k < n; i++, k += l) {
        GNA2D1::bf[i].set(p.data() + k, 0, s = std::min(l, n - k));
        GNA2D1::bf[i].zeros(s, l2), GNA2D1::bf[i].dft(0, l2);
      }
      if (&p != &q)
        for (int i = 0, k = 0, s; k < m; i++, k += l) {
          GNA2D2::bf[i].set(q.data() + k, 0, s = std::min(l, m - k));
          GNA2D2::bf[i].zeros(s, l2), GNA2D2::bf[i].dft(0, l2);
        }
      else
        for (int i = 0; i < nn; i++) GNA2D2::bf[i].subst(GNA2D1::bf[i], 0, l2);
      GNA2D2::bf[mm].zeros(0, l2);
      for (int i = mm; i--;)
        GNA2D2::bf[i + 1].add(GNA2D2::bf[i], 0, l),
            GNA2D2::bf[i + 1].dif(GNA2D2::bf[i], l, l2);
      for (int i = 0, k = 0, j, ed; k < sz; i++, k += l) {
        GNA2::bf.zeros(0, l2);
        for (j = std::max(0, i - nn + 1), ed = std::min(mm, i); j <= ed; j++)
          GNA1::bf.mul(GNA2D1::bf[i - j], GNA2D2::bf[j], 0, l2),
              GNA2::bf.add(GNA1::bf, 0, l2);
        GNA2::bf.idft(0, l2);
        GNA2::bf.get(GAr::bf + k, 0, std::min(l, sz - k));
      }
    } else {
      using GNA1 = GlobalNTTArray<mod_t, LIM, 1>;
      using GNA2 = GlobalNTTArray<mod_t, LIM, 2>;
      const int len = sz <= l + fl ? l : rl;
      GNA1::bf.set(p.data(), 0, n), GNA1::bf.zeros(n, len);
      if (GNA1::bf.dft(0, len); &p != &q) {
        GNA2::bf.set(q.data(), 0, m), GNA2::bf.zeros(m, len);
        GNA2::bf.dft(0, len), GNA1::bf.mul(GNA2::bf, 0, len);
      } else
        GNA1::bf.mul(GNA1::bf, 0, len);
      GNA1::bf.idft(0, len), GNA1::bf.get(GAr::bf, 0, std::min(sz, len));
      if (len < sz) {
        std::copy(p.begin() + len - m + 1, p.end(), GAp::bf + len - m + 1);
        std::copy(q.begin() + len - n + 1, q.end(), GAq::bf + len - n + 1);
        for (int i = len, j; i < sz; GAr::bf[i - len] -= GAr::bf[i], i++)
          for (GAr::bf[i] = 0, j = i - m + 1; j < n; j++)
            GAr::bf[i] += GAp::bf[j] * GAq::bf[i - j];
      }
    }
  }
  return std::vector<mod_t>(GAr::bf, GAr::bf + sz);
}
