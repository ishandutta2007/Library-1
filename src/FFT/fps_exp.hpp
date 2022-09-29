#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_div.hpp"

/**
 * @title 形式的冪級数 log, exp, pow
 * @category FFT
 */

// BEGIN CUT HERE
template <class mod_t, std::size_t LIM>
mod_t get_inv(int n) {
  static_assert(is_staticmodint_v<mod_t>);
  static constexpr auto m = mod_t::modulo();
  static mod_t dat[LIM] = {0, 1};
  static int l = 2;
  while (l <= n) dat[l++] = dat[m % l] * (m - m / l);
  return dat[n];
}

template <class mod_t>
std::vector<mod_t> deriv(const std::vector<mod_t> &p) {
  std::vector<mod_t> ret(p.size() - 1);
  for (int i = p.size(); --i;) ret[i - 1] = p[i] * i;
  return ret;
}
template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> integ(const std::vector<mod_t> &p) {
  std::vector<mod_t> ret(p.size() + 1, mod_t(0));
  for (int i = p.size(); i; i--) ret[i] = p[i - 1] * get_inv<mod_t, _Nm>(i);
  return ret;
}
template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> log(const std::vector<mod_t> &p) {
  assert(p[0] == mod_t(1));
  return integ<mod_t, _Nm>(div<mod_t, _Nm>(deriv(p), p));
}

template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> exp(const std::vector<mod_t> &p) {
  static constexpr int _Nm2 = _Nm * 2 / 15;
  static constexpr int TH = 64 << ((!is_nttfriend<mod_t, _Nm>()) << 1);
  using GAdp = GlobalArray<mod_t, _Nm, 1>;
  using GAr = GlobalArray<mod_t, _Nm, 2>;
  using GA = GlobalArray<mod_t, _Nm2, 0>;
  using GNA1 = GlobalNTTArray<mod_t, _Nm2, 1>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm2, 2>;
  using GNA2D1 = GlobalNTTArray2D<mod_t, _Nm2, 16, 1>;
  using GNA2D2 = GlobalNTTArray2D<mod_t, _Nm2, 16, 2>;
  const int n = p.size(), m = get_len(n);
  assert(n > 0), assert(p[0] == mod_t(0));
  std::copy(p.begin(), p.end(), GAdp::bf);
  for (int i = n; --i;) GAdp::bf[i] *= i;
  std::fill_n(GAr::bf, n, mod_t(0)), GAr::bf[0] = 1;
  for (int r = m, d = 0, R, k, i; r > TH; d += k) {
    k = (r /= (R = get_len(bsf(r) + 1) >> 1)) << 1;
    for (i = std::min(R - 1, (n - 1) / r); i--;)
      GNA2D1::bf[i].set(GAdp::bf + i * r - d, d, d + k),
          GNA2D1::bf[i].dft(d, d + k);
  }
  auto rec = [&](auto f, int l, int r, int d) -> void {
    if (int i = l | (!l), ed = std::min(r, n), j; r - l > TH) {
      int R = get_len(bsf(r - l) + 1) >> 1, len = (r - l) / R, k = len << 1;
      for (i = 0, ed = std::min(R, (n - l + len - 1) / len);; i++) {
        if (mod_t *ret = GAr::bf + l + i * len, *bf = GA::bf + d + len; i) {
          for (GNA1::bf.zeros(d, d + k), j = i; j--;)
            GNA2::bf.mul(GNA2D2::bf[j], GNA2D1::bf[i - j - 1], d, d + k),
                GNA1::bf.add(GNA2::bf, d, d + k);
          GNA1::bf.idft(d, d + k), GNA1::bf.get(GA::bf, d + len, d + k);
          for (int t = len; t--;) ret[t] += bf[t];
        }
        if (f(f, l + i * len, l + (i + 1) * len, d + k); i == ed - 1) break;
        GNA2D2::bf[i].set(GAr::bf + l + i * len - d, d, d + len);
        GNA2D2::bf[i].zeros(d + len, d + k), GNA2D2::bf[i].dft(d, d + k);
      }
    } else
      for (; i < ed; GAr::bf[i] *= get_inv<mod_t, _Nm>(i), i++)
        for (j = l; j < i; j++) GAr::bf[i] += GAr::bf[j] * GAdp::bf[i - j];
  };
  return rec(rec, 0, m, 0), std::vector<mod_t>(GAr::bf, GAr::bf + n);
}

template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> pow(const std::vector<mod_t> &p, std::uint64_t k) {
  using GA = GlobalArray<mod_t, _Nm, 4>;
  const mod_t Z(0), MK(k);
  int n = p.size(), cnt = 0;
  if (GA::bf[0] = 1; k) {
    while (cnt < n && p[cnt] == Z) cnt++;
    const __int128_t ofs = (__int128_t)k * cnt, sz = n - ofs;
    if (sz <= 0) return std::vector<mod_t>(n, Z);
    const mod_t p0 = p[cnt], iv = mod_t(1) / p0, pk = p0.pow(k);
    for (int i = sz; --i;) GA::bf[i] = p[i + cnt] * iv;
    auto q = log<mod_t, _Nm>(std::vector<mod_t>(GA::bf, GA::bf + sz));
    for (int i = sz; --i;) q[i] *= MK;
    std::copy_n(exp<mod_t, _Nm>(q).begin(), (int)sz, GA::bf + ofs);
    std::fill_n(GA::bf, (int)ofs, Z);
    for (int i = sz; i--;) GA::bf[i + ofs] *= pk;
  } else
    std::fill_n(GA::bf + 1, n - 1, Z);
  return std::vector<mod_t>(GA::bf, GA::bf + n);
}