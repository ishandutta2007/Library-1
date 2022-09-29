#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"

/**
 * @title sample points shift
 * @category FFT
 * O( (d+m)log(d+m) )
 */

// BEGIN CUT HERE
template <class mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> sample_points_shift(const std::vector<mod_t> &pts, mod_t c,
                                       int m) {
  assert(m <= mod_t::modulo()), assert(pts.size() <= mod_t::modulo());
  if (m == 0) return {};
  std::uint64_t c_64 = c.val(), nc1 = (c + (m - 1)).val();
  std::uint32_t k = pts.size(), d = k - 1, i = d, e;
  if (c_64 + m <= k)
    return std::vector<mod_t>(pts.begin() + c_64, pts.begin() + c_64 + m);
  using GA = GlobalArray<mod_t, _Nm, 0>;
  for (GA::bf[d] = 1; i; i--) GA::bf[i - 1] = GA::bf[i] * i;
  mod_t t = mod_t(1) / (GA::bf[0] * GA::bf[0]);
  for (i = d / 2 + 1; i--;)
    GA::bf[i] = GA::bf[d - i] = GA::bf[i] * GA::bf[d - i] * t;
  for (i = k; i--;) GA::bf[i] *= pts[i];
  for (i = 1; i < k; i += 2) GA::bf[d - i] = -GA::bf[d - i];
  const mod_t Z = 0;
  auto f = [&](mod_t a, int n, mod_t ret[]) {
    using GNA1 = GlobalNTTArray<mod_t, _Nm, 1>;
    using GNA2 = GlobalNTTArray<mod_t, _Nm, 2>;
    using GAq = GlobalArray<mod_t, _Nm, 2>;
    for (e = d + n, i = 0, t = a - d; i < e; i++, t += 1) ret[i] = t;
    std::partial_sum(ret, ret + e, GAq::bf, std::multiplies<>());
    for (t = mod_t(1) / GAq::bf[e - 1]; --i;)
      GAq::bf[i] = t * GAq::bf[i - 1], t *= ret[i];
    if (GAq::bf[0] = t; k >= 74 && n >= 128) {
      const int len = get_len(e + (d > 0));
      GNA1::bf.set(GA::bf, 0, k), GNA1::bf.zeros(k, len), GNA1::bf.dft(0, len);
      GNA2::bf.set(GAq::bf, 0, e), GNA2::bf.zeros(e, len), GNA2::bf.dft(0, len);
      GNA1::bf.mul(GNA2::bf, 0, len), GNA1::bf.idft(0, len);
      GNA1::bf.get(ret - d, d, e);
    } else
      for (std::fill_n(ret, n, Z), i = k; i--;)
        for (int b = d - i, j = n; j--;) ret[j] += GA::bf[i] * GAq::bf[j + b];
    for (t = a, i = k; --i;) t *= a - i;
    for (; i < n; i++) ret[i] *= t, t *= (a + (i + 1)) * GAq::bf[i];
    return ret + n;
  };
  using GAp = GlobalArray<mod_t, _Nm, 1>;
  if (mod_t * bf; c_64 < k) {
    if (bf = std::copy_n(pts.begin() + c_64, k - c_64, GAp::bf); nc1 < k)
      std::copy_n(pts.begin(), nc1 + 1, f(k, mod_t::modulo() - k, bf));
    else
      f(k, c_64 + m - k, bf);
  } else if (nc1 < c_64) {
    if (mod_t *bf = f(c, (-c).val(), GAp::bf); nc1 < k)
      std::copy_n(pts.begin(), nc1 + 1, bf);
    else
      f(k, nc1 + 1 - k, std::copy_n(pts.begin(), k, bf));
  } else
    f(c, m, GAp::bf);
  return std::vector<mod_t>(GAp::bf, GAp::bf + m);
}