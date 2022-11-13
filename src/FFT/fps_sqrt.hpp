#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_inv.hpp"

/**
 * @title 形式的冪級数 sqrt
 * @category FFT
 */

// BEGIN CUT HERE
template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> sqrt(const std::vector<mod_t> &p) {
  using GAp = GlobalArray<mod_t, _Nm, 1>;
  using GAr = GlobalArray<mod_t, _Nm, 2>;
  using GA3 = GlobalArray<mod_t, _Nm, 3>;
  using GA = GlobalArray<mod_t, _Nm, 0>;
  static constexpr std::size_t _Nm2 = _Nm * 2 / 15;
  using GNA1 = GlobalNTTArray<mod_t, _Nm2, 1>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm2, 2>;
  using GNA3 = GlobalNTTArray<mod_t, _Nm2, 3>;
  using GNA2D1 = GlobalNTTArray2D<mod_t, _Nm2, 16, 1>;
  static constexpr int TH = 128 << ((!is_nttfriend<mod_t, _Nm2>()) << 1);
  const mod_t Z(0), miv2((mod_t::modulo() - 1) >> 1);
  std::copy(p.begin(), p.end(), GAp::bf);
  int n = p.size(), cnt = 0;
  while (cnt < n && GAp::bf[cnt] == Z) cnt++;
  if (cnt == n) return p;
  if (cnt & 1) return {};  // no solution
  mod_t *bf = GAr::bf + (cnt >> 1), *pbf = GAp::bf + cnt;
  std::fill_n(GAr::bf, n, Z), bf[0] = pbf[0].sqrt();
  if (bf[0] * bf[0] != pbf[0]) return {};  // no solution
  const int nn = n - cnt, len = get_len(nn),
            R = get_len(__builtin_ctz(len) + 1) >> 1;
  int m = TH >= nn ? nn : len, lnR = __builtin_ctz(R), i = 2;
  while (m > TH) m >>= lnR;
  const mod_t miv = mod_t(mod_t::modulo() - 1) / (bf[0] + bf[0]);
  for ((bf[1] -= pbf[1]) *= miv; i < m; (bf[i] -= pbf[i]) *= miv, i++) {
    for (int j = (i + 1) / 2; --j;) bf[i] += bf[j] * bf[i - j];
    if (bf[i] += bf[i]; !(i & 1)) bf[i] += bf[i >> 1] * bf[i >> 1];
  }
  if (m == nn) return std::vector<mod_t>(GAr::bf, GAr::bf + n);
  for (GA3::bf[0] = mod_t(1) / bf[0], i = 1; m < nn; i = m, m <<= lnR) {
    int m2 = m << 1, ed = std::min(R, (nn + m - 1) / m), k = 1, mm = m, j;
    inv_base<_Nm2>(bf, m, GA3::bf, i);
    GNA1::bf.set(GA3::bf, 0, m), GNA1::bf.zeros(m, m2), GNA1::bf.dft(0, m2);
    for (mod_t *bfk = bf, *pbfk = pbf; k < ed; mm = std::min(m, nn - m * ++k)) {
      GNA2D1::bf[k - 1].set(bfk, 0, m), GNA2D1::bf[k - 1].zeros(m, m2);
      GNA2D1::bf[k - 1].dft(0, m2);
      for (GNA2::bf.zeros(0, m2), j = (k - 1) >> 1; j--;)
        GNA3::bf.add(GNA2D1::bf[j + 1], GNA2D1::bf[j], 0, m),
            GNA3::bf.dif(GNA2D1::bf[j + 1], GNA2D1::bf[j], m, m2),
            GNA3::bf.mul(GNA2D1::bf[k - 1 - j], 0, m2),
            GNA2::bf.add(GNA3::bf, 0, m2);
      if (j = k >> 1; !(k & 1))
        GNA3::bf.mul(GNA2D1::bf[j], GNA2D1::bf[k - 1 - j], 0, m2),
            GNA2::bf.add(GNA3::bf, 0, m), GNA2::bf.dif(GNA3::bf, m, m2);
      if (k != 1) GNA2::bf.add(GNA2::bf, 0, m2);
      GNA3::bf.mul(GNA2D1::bf[j], GNA2D1::bf[j], 0, m2), pbfk += m;
      k & 1 ? GNA2::bf.dif(GNA3::bf, m, m2) : GNA2::bf.add(GNA3::bf, m, m2);
      GNA2::bf.add(GNA3::bf, 0, m), GNA2::bf.idft(0, m2), GNA2::bf.zeros(m, m2);
      for (GNA2::bf.get(GA::bf, 0, mm), j = mm; j--;) GA::bf[j] -= pbfk[j];
      GNA2::bf.set(GA::bf, 0, mm), GNA2::bf.dft(0, m2);
      GNA2::bf.mul(GNA1::bf, 0, m2), GNA2::bf.idft(0, m2);
      for (GNA2::bf.get(bfk += m, 0, mm); mm--;) bfk[mm] *= miv2;
    }
  }
  return std::vector<mod_t>(GAr::bf, GAr::bf + n);
}