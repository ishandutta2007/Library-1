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
  using PMat = std::array<Poly, 4>;
  assert(a.deg() >= 0), assert(b.deg() >= 0);
  auto isI = [](const PMat &m) {
    const mod_t ONE(1);
    return m[1].deg() == -1 && m[2].deg() == -1 && m[0].deg() == 0 &&
           m[0][0] == ONE && m[3].deg() == 0 && m[3][0] == ONE;
  };
  auto mul = [&](const PMat &l, const PMat &r) -> PMat {
    if (isI(l)) return r;
    if (isI(r)) return l;
    return {l[0] * r[0] + l[1] * r[2], l[0] * r[1] + l[1] * r[3],
            l[2] * r[0] + l[3] * r[2], l[2] * r[1] + l[3] * r[3]};
  };
  auto mulQ_l = [&](const Poly &q, const PMat &r) -> PMat {
    return {r[2], r[3], r[0] - q * r[2], r[1] - q * r[3]};
  };
  auto hgcd = [&](auto self, const Poly &p0, const Poly &p1) -> PMat {
    assert(p0.deg() > p1.deg());
    int m = ((p0.deg() - 1) >> 1) + 1, n = p1.deg();
    if (n < m) return {Poly{1}, Poly(), Poly(), Poly{1}};
    PMat R = self(self, Poly(p0.begin() + m, p0.end()),
                  Poly(p1.begin() + m, p1.end()));
    Poly b = R[2] * p0 + R[3] * p1;
    if (b.deg() < m) return R;
    std::pair<Poly, Poly> qr = (R[0] * p0 + R[1] * p1).quorem(b);
    int k = 2 * m - b.deg();
    if ((int)qr.second.size() <= k) return mulQ_l(qr.first, R);
    return mul(self(self, Poly(b.begin() + k, b.end()),
                    Poly(qr.second.begin() + k, qr.second.end())),
               mulQ_l(qr.first, R));
  };
  auto cogcd = [&](auto self, const Poly &p0, const Poly &p1) -> PMat {
    assert(p0.deg() > p1.deg());
    PMat M = hgcd(hgcd, p0, p1);
    Poly p3 = M[2] * p0 + M[3] * p1;
    if (p3.deg() == -1) return M;
    std::pair<Poly, Poly> qr = (M[0] * p0 + M[1] * p1).quorem(p3);
    if (qr.second.deg() == -1) return mulQ_l(qr.first, M);
    return mul(self(self, p3, qr.second), mulQ_l(qr.first, M));
  };
  if (a.shrink().size() <= b.shrink().size()) {
    std::pair<Poly, Poly> qr = a.quorem(b);
    PMat c = cogcd(cogcd, b, qr.second);
    return a * (x = c[1]) + b * (y = c[0] - c[1] * qr.first);
  } else {
    PMat c = cogcd(cogcd, a, b);
    return a * (x = c[0]) + b * (y = c[1]);
  }
}