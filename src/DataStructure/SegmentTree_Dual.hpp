#pragma once
#include <bits/stdc++.h>
/**
 * @title Segment-Tree(双対)
 * @category データ構造
 * @brief O(logN)
 */

// BEGIN CUT HERE

template <typename M>
struct SegmentTree_Dual {
  using T = typename M::T;
  using E = typename M::E;
  SegmentTree_Dual() {}
  SegmentTree_Dual(int n_, T v1 = T())
      : n(n_),
        height(ceil(log2(n))),
        val(n, v1),
        laz(n * 2),
        laz_flg(n * 2, false) {}
  SegmentTree_Dual(const std::vector<T> &v)
      : n(v.size()),
        height(ceil(log2(n))),
        val(v),
        laz(n * 2),
        laz_flg(n * 2, false) {}
  void apply(int a, int b, E x) {
    a += n, b += n;
    for (int i = height; i >= 1; i--)
      if (((a >> i) << i) != a) eval(a >> i);
    for (int i = height; i >= 1; i--)
      if (((b >> i) << i) != b) eval((b - 1) >> i);
    for (int l = a, r = b; l < r; l >>= 1, r >>= 1) {
      if (l & 1) propagate(l++, x);
      if (r & 1) propagate(--r, x);
    }
  }
  void set(int k, T x) {
    for (int i = height; i; i--) eval((k + n) >> i);
    val[k] = x, laz_flg[k + n] = false;
  }
  T operator[](const int k) {
    for (int i = height; i; i--) eval(k >> i);
    if (laz_flg[k + n])
      val[k] = M::mapping(val[k], laz[k + n]), laz_flg[k + n] = false;
    return val[k];
  }

 private:
  const int n, height;
  std::vector<T> val;
  std::vector<E> laz;
  std::vector<char> laz_flg;
  inline void eval(int k) {
    if (!laz_flg[k]) return;
    propagate(k << 1 | 0, laz[k]), propagate(k << 1 | 1, laz[k]);
    laz_flg[k] = false;
  }
  inline void propagate(int k, const E &x) {
    laz[k] = laz_flg[k] ? M::composition(laz[k], x) : x, laz_flg[k] = true;
  }
};