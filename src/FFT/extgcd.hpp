#pragma once
#include <bits/stdc++.h>
#include "src/FFT/Polynomial.hpp"
/**
 * @title 多項式の拡張互除法
 * @category FFT
 *  O(N log^2 N)
 */

// BEGIN CUT HERE

// ax + by = gcd(a, b)
template <class mod_t, std::size_t _Nm>
Polynomial<mod_t, _Nm> extgcd(Polynomial<mod_t, _Nm> a,
                              Polynomial<mod_t, _Nm> b,
                              Polynomial<mod_t, _Nm> &x,
                              Polynomial<mod_t, _Nm> &y) {
  using Poly = Polynomial<mod_t, _Nm>;
  using PolyMat = std::array<Poly, 4>;
  assert(a.deg() >= 0), assert(b.deg() >= 0);
  auto isI = [](const PolyMat &m) {
    const mod_t ONE(1);
    return m[1].deg() == -1 && m[2].deg() == -1 && m[0].deg() == 0 &&
           m[0][0] == ONE && m[3].deg() == 0 && m[3][0] == ONE;
  };
  auto mul = [&](const PolyMat &l, const PolyMat &r) -> PolyMat {
    if (isI(l)) return r;
    if (isI(r)) return l;
    return {l[0] * r[0] + l[1] * r[2], l[0] * r[1] + l[1] * r[3],
            l[2] * r[0] + l[3] * r[2], l[2] * r[1] + l[3] * r[3]};
  };
  auto mulQ_l = [&](const Poly &q, const PolyMat &r) -> PolyMat {
    return {r[2], r[3], r[0] - q * r[2], r[1] - q * r[3]};
  };
#define SUF(f, k) Poly(f.begin() + k, f.end())
  auto hgcd = [&](auto self, const Poly &p0, const Poly &p1) -> PolyMat {
    assert(p0.deg() > p1.deg());
    int m = ((p0.deg() - 1) >> 1) + 1, n = p1.deg();
    if (n < m) return {mod_t(1), Poly(), Poly(), mod_t(1)};
    PolyMat R = self(self, SUF(p0, m), SUF(p1, m));
    Poly b = R[2] * p0 + R[3] * p1;
    if (b.deg() < m) return R;
    std::pair<Poly, Poly> qr = (R[0] * p0 + R[1] * p1).quorem(b);
    if (int k = 2 * m - b.deg(); (int)qr.second.size() > k)
      return mul(self(self, SUF(b, k), SUF(qr.second, k)), mulQ_l(qr.first, R));
    return mulQ_l(qr.first, R);
  };
  auto cogcd = [&](auto self, const Poly &p0, const Poly &p1) -> PolyMat {
    assert(p0.deg() > p1.deg());
    PolyMat M = hgcd(hgcd, p0, p1);
    Poly p3 = M[2] * p0 + M[3] * p1;
    if (p3.deg() == -1) return M;
    std::pair<Poly, Poly> qr = (M[0] * p0 + M[1] * p1).quorem(p3);
    if (qr.second.deg() == -1) return mulQ_l(qr.first, M);
    return mul(self(self, p3, qr.second), mulQ_l(qr.first, M));
  };
#undef SUF
  if (a.shrink().size() <= b.shrink().size()) {
    std::pair<Poly, Poly> qr = a.quorem(b);
    PolyMat c = cogcd(cogcd, b, qr.second);
    return a * (x = c[1]) + b * (y = c[0] - c[1] * qr.first);
  } else {
    PolyMat c = cogcd(cogcd, a, b);
    return a * (x = c[0]) + b * (y = c[1]);
  }
}