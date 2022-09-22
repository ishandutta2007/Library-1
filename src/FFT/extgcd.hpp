#pragma once
#include <bits/stdc++.h>
#include "src/FFT/Polynomial.hpp"
/**
 * @title 多項式の拡張互除法
 * @category 数学
 *  O(Nlog^2N)
 * @see https://loj.ac/article/2773
 */

// BEGIN CUT HERE

// ax + by = gcd(a, b)
template <class mod_t, std::size_t _Nm>
Polynomial<mod_t, _Nm> extgcd(Polynomial<mod_t, _Nm> a,
                              Polynomial<mod_t, _Nm> b,
                              Polynomial<mod_t, _Nm> &x,
                              Polynomial<mod_t, _Nm> &y) {
  using Poly = Polynomial<mod_t, _Nm>;
  using PVec = std::array<Poly, 2>;
  using PMat = std::array<PVec, 2>;
  assert(a.deg() >= 0);
  assert(b.deg() >= 0);
  auto isI = [](const PMat &m) {
    static constexpr mod_t ONE(1);
    return m[0][1].deg() == -1 && m[1][0].deg() == -1 && m[0][0].deg() == 0 &&
           m[0][0][0] == ONE && m[1][1].deg() == 0 && m[1][1][0] == ONE;
  };
  auto mulv = [&](const PMat &lhs, const PVec &rhs) {
    if (isI(lhs)) return rhs;
    return PVec{lhs[0][0] * rhs[0] + lhs[0][1] * rhs[1],
                lhs[1][0] * rhs[0] + lhs[1][1] * rhs[1]};
  };
  auto mul = [&](const PMat &lhs, const PMat &rhs) {
    if (isI(lhs)) return rhs;
    if (isI(rhs)) return lhs;
    return PMat{PVec{lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0],
                     lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1]},
                PVec{lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0],
                     lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1]}};
  };
  auto mulQ_l = [&](const Poly &q, const PMat &rhs) {
    return PMat{PVec{rhs[1][0], rhs[1][1]},
                PVec{rhs[0][0] - q * rhs[1][0], rhs[0][1] - q * rhs[1][1]}};
  };
  auto hgcd = [&](auto self, const Poly &p0, const Poly &p1) -> PMat {
    assert(p0.deg() > p1.deg());
    int m = ((p0.deg() - 1) >> 1) + 1, n = p1.deg();
    if (n < m) return PMat{PVec{Poly(1, 1), Poly()}, PVec{Poly(), Poly(1, 1)}};
    PMat R(self(self, Poly(p0.begin() + m, p0.end()),
                Poly(p1.begin() + m, p1.end())));
    PVec ab(mulv(R, PVec{p0, p1}));
    if (ab[1].deg() < m) return R;
    std::pair<Poly, Poly> qr(ab[0].quorem(ab[1]));
    int k = 2 * m - ab[1].deg();
    if ((int)qr.second.size() <= k) return mulQ_l(qr.first, R);
    return mul(self(self, Poly(ab[1].begin() + k, ab[1].end()),
                    Poly(qr.second.begin() + k, qr.second.end())),
               mulQ_l(qr.first, R));
  };
  auto cogcd = [&](const Poly &p0, const Poly &p1) -> PMat {
    assert(p0.deg() > p1.deg());
    PMat M(hgcd(hgcd, p0, p1));
    PVec p2p3(mulv(M, PVec{p0, p1}));
    if (p2p3[1].deg() == -1) return M;
    std::pair<Poly, Poly> qr(p2p3[0].quorem(p2p3[1]));
    if (qr.second.deg() == -1) return mulQ_l(qr.first, M);
    return mul(self(self, p2p3[1], qr.second), mulQ_l(qr.first, M));
  };
  if (a.shrink().deg() <= b.shrink().deg()) {
    std::pair<Poly, Poly> qr(a.quorem(b));
    PMat c(cogcd(b, qr.second));
    return a * (x = c[0][1]) + b * (y = c[0][0] - c[0][1] * qr.first);
  } else {
    PMat c(cogcd(a, b));
    return a * (x = c[0][0]) + b * (y = c[0][1]);
  }
}
