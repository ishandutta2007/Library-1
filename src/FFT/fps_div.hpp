#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_inv.hpp"

/**
 * @title 形式的冪級数 div
 * @category FFT
 */

// BEGIN CUT HERE
template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> div(const std::vector<mod_t> &p,
                       const std::vector<mod_t> &q) {
  using GAp = GlobalArray<mod_t, _Nm, 0>;
  using GAq = GlobalArray<mod_t, _Nm, 1>;
  using GAr = GlobalArray<mod_t, _Nm, 2>;
  using GA = GlobalArray<mod_t, _Nm, 3>;
  static constexpr std::size_t _Nm2 = _Nm * 2 / 15;
  using GNA1 = GlobalNTTArray<mod_t, _Nm2, 1>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm2, 2>;
  using GNA3 = GlobalNTTArray<mod_t, _Nm2, 3>;
  using GNA2D1 = GlobalNTTArray2D<mod_t, _Nm2, 16, 1>;
  using GNA2D2 = GlobalNTTArray2D<mod_t, _Nm2, 16, 2>;
  static constexpr int TH = 128 << (!is_nttfriend<mod_t, _Nm2>());
  static constexpr int TH2 = 1024 << (!is_nttfriend<mod_t, _Nm2>());
  static constexpr int A = is_nttfriend<mod_t, _Nm2>()      ? 7
                           : is_nttarraydouble<mod_t, _Nm2> ? 17
                                                            : 18;
  static constexpr int B = is_nttfriend<mod_t, _Nm2>()      ? 29
                           : is_nttarraydouble<mod_t, _Nm2> ? 85
                                                            : 129;
  const int n = p.size(), len = get_len(n), R = len < TH2 ? 8 : 16;
  const int l = q.size(), lnR = __builtin_ctz(R);
  std::copy(p.begin(), p.end(), GAp::bf);
  std::copy(q.begin(), q.end(), GAq::bf);
  const mod_t iv0 = mod_t(1) / GAq::bf[0];
  if (l < A * __builtin_ctz(len) + B || TH >= n) {
    for (int i = 0; i < n; GAp::bf[i++] *= iv0)
      for (int j = std::min(i + 1, l); --j;)
        GAp::bf[i] -= GAp::bf[i - j] * GAq::bf[j];
    return std::vector<mod_t>(GAp::bf, GAp::bf + n);
  }
  int m = len, i = 0;
  while (m > TH) m >>= lnR;
  for (std::copy_n(GAp::bf, m, GAr::bf); i < m; GAr::bf[i++] *= iv0)
    for (int j = std::min(i + 1, l); --j;)
      GAr::bf[i] -= GAr::bf[i - j] * GAq::bf[j];
  if (l < n) std::fill(GAq::bf + l, GAq::bf + n, mod_t(0));
  for (GA::bf[0] = iv0, i = 1; m < n; i = m, m <<= lnR) {
    int m2 = m << 1, ed = std::min(R, (n + m - 1) / m), k = 1, j, mm = m;
    inv_base<_Nm2>(GAq::bf, m, GA::bf, i);
    GNA1::bf.set(GA::bf, 0, m), GNA1::bf.zeros(m, m2), GNA1::bf.dft(0, m2);
    mod_t *bfk = GAr::bf, *qbfk = GAq::bf, *pbfk = GAp::bf;
    GNA2D2::bf[0].set(qbfk, 0, m), GNA2D2::bf[0].zeros(m, m2);
    for (GNA2D2::bf[0].dft(0, m2); k < ed; mm = std::min(m, n - m * ++k)) {
      GNA2D1::bf[k - 1].set(bfk, 0, m), GNA2D1::bf[k - 1].zeros(m, m2);
      GNA2D1::bf[k - 1].dft(0, m2);
      GNA3::bf.set(qbfk += m, 0, m), GNA3::bf.zeros(m, m2), GNA3::bf.dft(0, m2);
      GNA2D2::bf[k].add(GNA3::bf, GNA2D2::bf[0], 0, m);
      GNA2D2::bf[k].dif(GNA3::bf, GNA2D2::bf[0], m, m2), GNA2::bf.zeros(0, m2);
      if (k + 1 < ed) GNA2D2::bf[0].subst(GNA3::bf, 0, m2);
      for (j = k; j--;)
        GNA3::bf.mul(GNA2D1::bf[j], GNA2D2::bf[k - j], 0, m2),
            GNA2::bf.add(GNA3::bf, 0, m2);
      GNA2::bf.idft(0, m2), GNA2::bf.zeros(m, m2), pbfk += m;
      for (GNA2::bf.get(bfk += m, 0, mm), j = mm; j--;) bfk[j] -= pbfk[j];
      GNA2::bf.set(bfk, 0, mm);
      GNA2::bf.dft(0, m2), GNA2::bf.mul(GNA1::bf, 0, m2), GNA2::bf.idft(0, m2);
      for (GNA2::bf.get(bfk, 0, mm); mm--;) bfk[mm] = -bfk[mm];
    }
  }
  return std::vector<mod_t>(GAr::bf, GAr::bf + n);
}