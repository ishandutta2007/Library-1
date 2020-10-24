#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
/**
 * @title 多項式の拡張互除法
 * @category 数学
 *  O(Nlog^2N)
 * @see https://loj.ac/article/2773
 */

// BEGIN CUT HERE

template <class mint>
FormalPowerSeries<mint> exgcd(
    FormalPowerSeries<mint> a, FormalPowerSeries<mint> b,
    FormalPowerSeries<mint> &x,
    FormalPowerSeries<mint> &y) {  // ax + by = gcd(a, b)
  using poly = FormalPowerSeries<mint>;
  using pv = std::array<poly, 2>;
  using pm = std::array<pv, 2>;
  assert(a.deg() >= 0);
  assert(b.deg() >= 0);
  auto isI = [](const pm &m) {
    static constexpr mint ONE(1);
    return m[0][1].deg() == -1 && m[1][0].deg() == -1 && m[0][0].deg() == 0
           && m[0][0][0] == ONE && m[1][1].deg() == 0 && m[1][1][0] == ONE;
  };
  auto mulv = [&](const pm &lhs, const pv &rhs) {
    if (isI(lhs)) return rhs;
    return pv{lhs[0][0] * rhs[0] + lhs[0][1] * rhs[1],
              lhs[1][0] * rhs[0] + lhs[1][1] * rhs[1]};
  };
  auto mul = [&](const pm &lhs, const pm &rhs) {
    if (isI(lhs)) return rhs;
    if (isI(rhs)) return lhs;
    return pm{pv{lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0],
                 lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1]},
              pv{lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0],
                 lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1]}};
  };
  auto mulQ_l = [&](const poly &q, const pm &rhs) {
    return pm{pv{rhs[1][0], rhs[1][1]},
              pv{rhs[0][0] - q * rhs[1][0], rhs[0][1] - q * rhs[1][1]}};
  };
  auto mulQ_r = [&](const pm &lhs, const poly &q) {
    return pm{pv{lhs[0][1], lhs[0][0] - lhs[0][1] * q},
              pv{lhs[1][1], lhs[1][0] - lhs[1][1] * q}};
  };
  std::function<pm(poly, poly)> hgcd = [&](const poly &p0, const poly &p1) {
    assert(p0.deg() > p1.deg());
    int m = ((p0.deg() - 1) >> 1) + 1, n = p1.deg();
    if (n < m) return pm{pv{poly{1}, poly{}}, pv{poly{}, poly{1}}};
    pm R(hgcd(poly(p0.begin() + m, p0.end()), poly(p1.begin() + m, p1.end())));
    pv ab(mulv(R, pv{p0, p1}));
    if (ab[1].deg() < m) return R;
    std::pair<poly, poly> qr(ab[0].quorem(ab[1]));
    int k = 2 * m - ab[1].deg();
    if ((int)qr.second.size() <= k) return mulQ_l(qr.first, R);
    return mul(hgcd(poly(ab[1].begin() + k, ab[1].end()),
                    poly(qr.second.begin() + k, qr.second.end())),
               mulQ_l(qr.first, R));
  };
  std::function<pm(poly, poly)> cogcd = [&](const poly &p0, const poly &p1) {
    assert(p0.deg() > p1.deg());
    pm M(hgcd(p0, p1));
    pv p2p3(mulv(M, pv{p0, p1}));
    if (p2p3[1].deg() == -1) return M;
    std::pair<poly, poly> qr(p2p3[0].quorem(p2p3[1]));
    if (qr.second.deg() == -1) return mulQ_l(qr.first, M);
    return mul(cogcd(p2p3[1], qr.second), mulQ_l(qr.first, M));
  };
  pm c;
  if (a.norm().deg() > b.norm().deg()) {
    c = cogcd(a, b);
  } else {
    std::pair<poly, poly> qr(a.quorem(b));
    c = mulQ_r(cogcd(b, qr.second), qr.first);
  }
  return a * (x = c[0][0]) + b * (y = c[0][1]);
}
