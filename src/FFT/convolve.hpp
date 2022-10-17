#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"

/**
 * @title 畳み込み
 * @category FFT
 */

// BEGIN CUT HERE
template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> convolve(const std::vector<mod_t> &p,
                            const std::vector<mod_t> &q) {
  using GNA1 = GlobalNTTArray<mod_t, _Nm, 1>;
  using GAr = GlobalArray<mod_t, _Nm, 0>;
  using GAp = GlobalArray<mod_t, _Nm, 1>;
  using GAq = GlobalArray<mod_t, _Nm, 2>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm, 2>;
  static constexpr int TH = 74, TMP = 7 * nttarray_type<mod_t, _Nm>;
  const int n = p.size(), m = q.size(), r_len = n + m - 1;
  if (!n || !m) return std::vector<mod_t>();
  if (std::min(n, m) < TH) {
    std::fill_n(GAr::bf, r_len, mod_t(0));
    std::copy(p.begin(), p.end(), GAp::bf);
    std::copy(q.begin(), q.end(), GAq::bf);
    for (int i = n; i--;)
      for (int j = m; j--;) GAr::bf[i + j] += GAp::bf[i] * GAq::bf[j];
  } else {
    const int l = get_len(std::max(n, m)),
              bl = __builtin_ctz(l) + 2 * nttarray_type<mod_t, _Nm> - 6;
    const int len = r_len - l < bl * bl * TMP - TH ? l : get_len(r_len);
    GNA1::bf.set(p.data(), 0, n), GNA1::bf.zeros(n, len), GNA1::bf.dft(0, len);
    if (&p == &q)
      GNA1::bf.mul(GNA1::bf, 0, len);
    else
      GNA2::bf.set(q.data(), 0, m), GNA2::bf.zeros(m, len),
          GNA2::bf.dft(0, len), GNA1::bf.mul(GNA2::bf, 0, len);
    GNA1::bf.idft(0, len), GNA1::bf.get(GAr::bf, 0, std::min(r_len, len));
    if (len < r_len) {
      std::copy(p.begin() + len - m + 1, p.end(), GAp::bf + len - m + 1);
      std::copy(q.begin() + len - n + 1, q.end(), GAq::bf + len - n + 1);
      for (int i = len, j; i < r_len; GAr::bf[i - len] -= GAr::bf[i], i++)
        for (GAr::bf[i] = 0, j = i - m + 1; j < n; j++)
          GAr::bf[i] += GAp::bf[j] * GAq::bf[i - j];
    }
  }
  return std::vector<mod_t>(GAr::bf, GAr::bf + r_len);
}
