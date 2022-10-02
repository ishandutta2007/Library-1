#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"

/**
 * @title 多変数畳み込み
 * @category FFT
 * @see https://37zigen.com/truncated-multivariate-convolution/
 */

// BEGIN CUT HERE
class MultiVariateConvolution {
  int n, k, m;
  std::vector<int> chi;

 public:
  MultiVariateConvolution() : MultiVariateConvolution(std::vector<int>{}) {}
  MultiVariateConvolution(const std::vector<int> &dim)
      : n(std::accumulate(dim.begin(), dim.end(), 1, std::multiplies<int>())),
        k(dim.size()),
        m(get_len(n) * 2),
        chi(n, 0) {
    for (int i = n; i--;)
      for (int den = 1, j = 0; j < k; j++) chi[i] += i / (den *= dim[j]);
    if (k)
      for (int i = n; i--;) chi[i] %= k;
  }
  int size() const { return n; }
  int dim() const { return k; }
  template <typename mod_t, std::size_t _Nm = 1 << 20, std::size_t _Nm2 = 20>
  std::vector<mod_t> convolve(const std::vector<mod_t> &f,
                              const std::vector<mod_t> &g) const {
    assert((int)f.size() == n), assert((int)g.size() == n);
    if (!k) return {f[0] * g[0]};
    using GA = GlobalArray<mod_t, _Nm, 0>;
    using GNA = GlobalNTTArray<mod_t, _Nm, 0>;
    using GNA2D = GlobalNTTArray2D<mod_t, _Nm, _Nm2, 0>;
    using GNA2D1 = GlobalNTTArray2D<mod_t, _Nm, _Nm2, 1>;
    using GNA2D2 = GlobalNTTArray2D<mod_t, _Nm, _Nm2, 2>;
    for (int i = k; i--;) GNA2D::bf[i].zeros(0, m);
    for (int i = k; i--;) GNA2D1::bf[i].zeros(0, m);
    for (int i = k; i--;) GNA2D2::bf[i].zeros(0, m);
    for (int i = n; i--;) GNA2D1::bf[chi[i]].set(i, f[i]);
    for (int i = n; i--;) GNA2D2::bf[chi[i]].set(i, g[i]);
    for (int i = k; i--;) GNA2D1::bf[i].dft(0, m);
    for (int i = k; i--;) GNA2D2::bf[i].dft(0, m);
    for (int i = k, j, r; i--;)
      for (j = k; j--;)
        GNA::bf.mul(GNA2D1::bf[i], GNA2D2::bf[j], 0, m),
            GNA2D::bf[r -= k & -((r = i + j) >= k)].add(GNA::bf, 0, m);
    for (int i = k; i--;) GNA2D::bf[i].idft(0, m);
    for (int i = n; i--;) GA::bf[i] = GNA2D::bf[chi[i]].get(i);
    return std::vector<mod_t>(GA::bf, GA::bf + n);
  }
};