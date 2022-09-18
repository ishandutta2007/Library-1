#pragma once
#include <bits/stdc++.h>
#include "src/Old/ModInt.hpp"
#include "src/Old/FormalPowerSeries.hpp"
/**
 * @title 微分方程式
 * @category 数学
 * @see https://nyaannyaan.github.io/library/fps/differential-equation.hpp
 * df/dx=F(f)
 */

// BEGIN CUT HERE

template <typename mint>
FormalPowerSeries<mint> differential_equation(
    std::function<FormalPowerSeries<mint>(FormalPowerSeries<mint>, int)> F,
    std::function<FormalPowerSeries<mint>(FormalPowerSeries<mint>, int)> dF,
    mint f0, int deg) {
  FormalPowerSeries<mint> f{f0};
  for (int e = 1, ne = 2; e < deg; e = ne, ne = std::min(e << 1, deg)) {
    auto a = dF(f, ne - 1), r = a.inte().exp(), h = a * f;
    h.resize(ne - 1), h = (F(f, ne - 1) - h).div(r).inte();
    f = (h + f0) * r, f.resize(ne);
  }
  return f;
}
