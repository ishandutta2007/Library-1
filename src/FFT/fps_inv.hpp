#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"

/**
 * @title 形式的冪級数 inv
 * @category FFT
 */

// BEGIN CUT HERE
namespace ntt_internal {
template <std::size_t _Nm, class mod_t>
void inv_base(const mod_t p[], int n, mod_t r[], int i = 1) {
  using GNA1 = GlobalNTTArray<mod_t, _Nm, 1>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm, 2>;
  static constexpr int TH = 64 << (!is_nttfriend<mod_t, _Nm>() << 1);
  if (n <= i) return;
  assert(((n & -n) == n)), assert(i && ((i & -i) == i));
  const int m = std::min(n, TH);
  const mod_t Z = 0, miv = -r[0];
  for (int j; i < m; r[i++] *= miv)
    for (r[j = i] = Z; j--;) r[i] += r[j] * p[i - j];
  for (int e = i << 1; i < n; e = i << 1) {
    GNA1::bf.set(r, 0, i), GNA1::bf.zeros(i, e), GNA1::bf.dft(0, e);
    GNA2::bf.set(p, 0, e), GNA2::bf.dft(0, e);
    GNA2::bf.mul(GNA1::bf, 0, e), GNA2::bf.idft(0, e);
    if constexpr (!is_nttfriend<mod_t, _Nm>())
      GNA2::bf.get(r, i, e), GNA2::bf.set(r, i, e);
    GNA2::bf.zeros(0, i), GNA2::bf.dft(0, e), GNA1::bf.mul(GNA2::bf, 0, e);
    for (GNA1::bf.idft(0, e), GNA1::bf.get(r, i, e); i < e; i++) r[i] = -r[i];
  }
}
template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> inv(const std::vector<mod_t> &p) {
  using GAp = GlobalArray<mod_t, _Nm, 1>;
  using GAr = GlobalArray<mod_t, _Nm, 2>;
  static constexpr std::size_t _Nm2 = _Nm * 2 / 15;
  using GNA1 = GlobalNTTArray<mod_t, _Nm2, 1>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm2, 2>;
  using GNA2D1 = GlobalNTTArray2D<mod_t, _Nm2, 16, 1>;
  using GNA2D2 = GlobalNTTArray2D<mod_t, _Nm2, 16, 2>;
  static constexpr int TH2 =
      is_nttfriend<mod_t, _Nm2>()
          ? 115
          : ((nttarray_type<mod_t, _Nm2>) == 2 ? 384 : 452);
  static constexpr int C = nttarray_type<mod_t, _Nm> << 1, lnR = 4;
  static constexpr int TH3 = 5 + ((nttarray_type<mod_t, _Nm> == 3) << 1);
  static constexpr int D = 10 * nttarray_type<mod_t, _Nm>;
  const int n = p.size();
  assert(n > 0), assert(p[0] != mod_t(0));
  std::copy(p.begin(), p.end(), GAp::bf);
  mod_t *bfk = GAr::bf, *pbfk = GAp::bf;
  const mod_t Z = 0, miv = -(bfk[0] = mod_t(1) / pbfk[0]);
  if (n < TH2) {
    for (int j, i = 1; i < n; bfk[i++] *= miv)
      for (bfk[j = i] = Z; j--;) bfk[i] += bfk[j] * pbfk[i - j];
    return std::vector<mod_t>(bfk, bfk + n);
  }
  const int bl = std::max(__builtin_ctz(get_len(n)) - lnR, TH3);
  int l = ((n >> bl) + ((((1 << bl) - 1) & n) > (C * bl + D))) << bl;
  const int m = l & -l, m2 = m << 1, ed = (l + m - 1) / m;
  if (inv_base<_Nm2>(pbfk, m, bfk); 1 < ed)
    GNA2D2::bf[0].set(pbfk, 0, m), GNA2D2::bf[0].zeros(m, m2),
        GNA2D2::bf[0].dft(0, m2);
  for (int k = 1, mm = m; k < ed; mm = std::min(m, n - m * ++k)) {
    GNA2D1::bf[k - 1].set(bfk, 0, m), GNA2D1::bf[k - 1].zeros(m, m2);
    GNA2D1::bf[k - 1].dft(0, m2), GNA1::bf.set(pbfk += m, 0, m);
    GNA1::bf.zeros(m, m2);
    GNA1::bf.dft(0, m2), GNA2D2::bf[k].add(GNA1::bf, GNA2D2::bf[0], 0, m);
    GNA2D2::bf[k].dif(GNA1::bf, GNA2D2::bf[0], m, m2), GNA2::bf.zeros(0, m2);
    if (k + 1 < ed) GNA2D2::bf[0].subst(GNA1::bf, 0, m2);
    for (int j = k; j--;)
      GNA1::bf.mul(GNA2D1::bf[j], GNA2D2::bf[k - j], 0, m2),
          GNA2::bf.add(GNA1::bf, 0, m2);
    GNA2::bf.idft(0, m2), GNA2::bf.zeros(m, m2), bfk += m;
    if constexpr (!is_nttfriend<mod_t, _Nm2>())
      GNA2::bf.get(bfk, 0, m), GNA2::bf.set(bfk, 0, m);
    GNA2::bf.dft(0, m2);
    GNA2::bf.mul(GNA2D1::bf[0], 0, m2), GNA2::bf.idft(0, m2);
    for (GNA2::bf.get(bfk, 0, mm); mm--;) bfk[mm] = -bfk[mm];
  }
  if (l < n)
    for (int j; l < n; GAr::bf[l++] *= miv)
      for (GAr::bf[j = l] = Z; j--;) GAr::bf[l] += GAr::bf[j] * GAp::bf[l - j];
  return std::vector<mod_t>(GAr::bf, GAr::bf + n);
}
}  // namespace ntt_internal
using ntt_internal::inv;