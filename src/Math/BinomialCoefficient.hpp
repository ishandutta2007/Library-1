#pragma once
#include <bits/stdc++.h>
#include "src/Math/Factors.hpp"
/**
 * @title 二項係数 ($\mathbb{Z}/m\mathbb{Z}$)
 * @category 数学
 * @see https://ferin-tech.hatenablog.com/entry/2018/01/17/010829
 */

// BEGIN CUT HERE
class BinomialCoefficient {  // mod <= 1e6
  static inline constexpr int inv_mod(int a, int mod) {
    int x = 1, y = 0, b = mod;
    for (int q = 0, z = 0, c = 0; b;)
      z = x, c = a, x = y, y = z - y * (q = a / b), a = b, b = c - b * q;
    return assert(a == 1), x < 0 ? mod - (-x) % mod : x % mod;
  }
  using i64 = std::int64_t;
  struct ModPe {
    ModPe() = default;
    ModPe(int p, int e, std::size_t pre_size = 1 << 14)
        : p(p), e(e), ppows(e + 1, 1) {
      for (int i = 1; i <= e; ++i) ppows[i] = ppows[i - 1] * p;
      for (pp = pe = ppows[e]; std::size_t(pp) * p <= pre_size;) pp *= p;
      q = pp / pe * p, facts.resize(pp, 1);
      for (int qq = 1, l = pp / p; qq < q; qq *= p, l /= p)
        for (int i = 0; i < l; ++i)
          for (int j = i * p + 1; j < i * p + p; ++j) facts[j * qq] = j;
      for (int i = 1; i < pp; ++i) facts[i] = i64(facts[i - 1]) * facts[i] % pe;
      mask = (facts[pp - 1] == pe - 1), ds.resize(q, 0);
      for (int i = 0; i < pp / pe; ++i)
        for (int j = 0, s = ds[i]; j < p; ++j) ds[i * p + j] = s + j;
    }
    int operator()(i64 n, i64 m) const {
      int num = 1, den = 1, x = 0, s = 0;
      if (i64 r = n - m; e > 1)
        for (i64 n1, m1, r1; n > 0; n = n1, m = m1, r = r1) {
          n1 = n / pp, m1 = m / pp, r1 = r / pp;
          num = i64(num) * facts[n - n1 * pp] % pp;
          den = i64(den) * facts[m - m1 * pp] % pp * facts[r - r1 * pp] % pp;
          s += n1 - m1 - r1, n1 = n / q, m1 = m / q, r1 = r / q;
          x += ds[m - m1 * q] + ds[r - r1 * q] - ds[n - n1 * q];
        }
      else
        for (i64 n1, m1, r1; n > 0; n = n1, m = m1, r = r1) {
          n1 = n / pp, m1 = m / pp, r1 = r / pp;
          int nr = n - n1 * pp, mr = m - m1 * pp, rr = r - r1 * pp;
          num = i64(num) * facts[nr] % pp;
          den = i64(den) * facts[mr] % pp * facts[rr] % pp;
          s += n1 - m1 - r1, x += ds[mr] + ds[rr] - ds[nr];
        }
      if (x >= e * (p - 1)) return 0;
      if (p > 2) x /= p - 1;
      int ret = i64(num) * inv_mod(den, pe) % pe * ppows[x] % pe;
      return (s & mask) && ret > 0 ? pe - ret : ret;
    }
    int p, e, mask, pe, q, pp;
    std::vector<int> ppows, facts, ds;
  };
  int mod;
  std::vector<ModPe> binom_pp;
  std::vector<int> iprods;

 public:
  BinomialCoefficient(int mod, std::size_t pre_size = 1 << 14) : mod(mod) {
    Factors f(mod);
    if (f.size() == 1) pre_size = 1 << 20;
    int prod = 1;
    for (auto [p, e] : f) {
      binom_pp.emplace_back(ModPe(p, e, pre_size));
      iprods.push_back(inv_mod(prod, binom_pp.back().pe));
      prod *= binom_pp.back().pe;
    }
  }
  inline int nCr(i64 n, i64 r) const {
    assert(r >= 0);
    if (n < r) return 0;
    if (r == 0) return (mod > 1);
    int ret = 0, prod = 1;
    for (size_t i = 0, d, ed = binom_pp.size(); i < ed; ++i, prod *= d)
      d = binom_pp[i].pe,
      ret += i64(binom_pp[i](n, r) + d - ret % d) * iprods[i] % d * prod;
    return ret;
  }
  inline int nHr(i64 n, i64 r) const { return !r ? 1 : nCr(n + r - 1, r); }
};