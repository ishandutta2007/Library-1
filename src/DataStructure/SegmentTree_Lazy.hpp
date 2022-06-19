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
  SegmentTree_Lazy() {}
  SegmentTree_Lazy(int n_)
      : n(n_),
        height(ceil(log2(n))),
        dat(n * 2, M::ti()),
        laz(n * 2),
        laz_flg(n * 2, false) {}
  SegmentTree_Lazy(int n_, T v1) : SegmentTree_Lazy(n_) {
    for (int i = n; i--;) dat[i + n] = v1;
    for (int i = n; --i;) update(i);
  }
  SegmentTree_Lazy(const std::vector<T> &v) : SegmentTree_Lazy(v.size()) {
    for (int i = n; i--;) dat[i + n] = v[i];
    for (int i = n; --i;) update(i);
  }
  void unsafe_set(int k, T x) { dat[k + n] = x; }
  void rebuild() {
    laz_flg.assign(n * 2, false);
    for (int i = n; --i;) update(i);
  }
  void apply(int a, int b, E x) {
    a += n, b += n;
    for (int i = height; i; i--)
      if (((a >> i) << i) != a) eval(a >> i);
    for (int i = height; i; i--)
      if (((b >> i) << i) != b) eval((b - 1) >> i);
    for (int l = a, r = b; l < r; l >>= 1, r >>= 1) {
      if (l & 1) propagate(l++, x);
      if (r & 1) propagate(--r, x);
    }
    for (int i = 1; a >> i; i++)
      if (((a >> i) << i) != a) update(a >> i);
    for (int i = 1; b >> i; i++)
      if (((b >> i) << i) != b) update((b - 1) >> i);
  }
  void set(int k, T x) {
    int i = height;
    for (k += n; i; i--) eval(k >> i);
    for (dat[k] = x, laz_flg[k] = false, i = 1; k >> i; i++) update(k >> i);
  }
  T fold(int a, int b) {  //[a,b)
    a += n, b += n;
    for (int i = height; i; i--)
      if (((a >> i) << i) != a) eval(a >> i);
    for (int i = height; i; i--)
      if (((b >> i) << i) != b) eval(b >> i);
    T vl = M::ti(), vr = M::ti();
    for (int l = a, r = b; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vl = M::op(vl, dat[l++]);
      if (r & 1) vr = M::op(dat[--r], vr);
    }
    return M::op(vl, vr);
  }
  T operator[](const int k) { return fold(k, k + 1); }

 private:
  const int n, height;
  std::vector<T> dat;
  std::vector<E> laz;
  std::vector<char> laz_flg;
  inline void eval(int k) {
    if (!laz_flg[k]) return;
    propagate(k << 1 | 0, laz[k]), propagate(k << 1 | 1, laz[k]);
    laz_flg[k] = false;
  }
  inline void propagate(int k, const E &x) {
    laz[k] = laz_flg[k] ? M::composition(laz[k], x) : x, laz_flg[k] = true;
    dat[k] = M::mapping(dat[k], x);
  }
  inline void update(int k) {
    dat[k] = M::op(dat[k << 1 | 0], dat[k << 1 | 1]);
  }
};