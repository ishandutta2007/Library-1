#pragma once
#include <bits/stdc++.h>
/**
 * @title Segment-Tree(遅延伝搬)
 * @category データ構造
 * @brief O(logN)
 */

// BEGIN CUT HERE

template <typename M>
struct SegmentTree_Lazy {
  using T = typename M::T;
  using E = typename M::E;

 private:
  const int height, n;
  std::vector<T> dat;
  std::vector<E> laz;

 private:
  inline T reflect(int k) {
    return laz[k] == M::ei() ? dat[k] : M::mapping(dat[k], laz[k]);
  }
  inline void eval(int k) {
    if (laz[k] == M::ei()) return;
    laz[(k << 1) | 0] = M::composition(laz[(k << 1) | 0], laz[k]);
    laz[(k << 1) | 1] = M::composition(laz[(k << 1) | 1], laz[k]);
    dat[k] = reflect(k);
    laz[k] = M::ei();
  }
  inline void thrust(int k) {
    for (int i = height; i; i--) eval(k >> i);
  }
  inline void recalc(int k) {
    while (k >>= 1) dat[k] = M::op(reflect((k << 1) | 0), reflect((k << 1) | 1));
  }

 public:
  SegmentTree_Lazy() {}
  SegmentTree_Lazy(int n_, T v1 = M::ti())
      : SegmentTree_Lazy(std::vector<T>(n_, v1)) {}
  SegmentTree_Lazy(const std::vector<T>& v)
      : height(ceil(log2(v.size()))),
        n(1 << height),
        dat(n * 2, M::ti()),
        laz(n * 2, M::ei()) {
    for (int i = 0; i < (int)v.size(); i++) dat[i + n] = v[i];
    for (int i = n - 1; i >= 1; i--)
      dat[i] = M::op(dat[i << 1 | 0], dat[i << 1 | 1]);
  }
  void apply(int a, int b, E x) {
    thrust(a += n);
    thrust(b += n - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) laz[l] = M::composition(laz[l], x), l++;
      if (r & 1) --r, laz[r] = M::composition(laz[r], x);
    }
    recalc(a);
    recalc(b);
  }
  void set_val(int a, T x) {
    thrust(a += n);
    dat[a] = x;
    laz[a] = M::ei();
    recalc(a);
  }
  //[a,b)
  T fold(int a, int b) {
    thrust(a += n);
    thrust(b += n - 1);
    T vl = M::ti(), vr = M::ti();
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vl = M::op(vl, reflect(l++));
      if (r & 1) vr = M::op(reflect(--r), vr);
    }
    return M::op(vl, vr);
  }
  T operator[](const int k) { return fold(k, k + 1); }
};