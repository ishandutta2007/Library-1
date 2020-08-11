/**
 * @title Segment-Tree(双対)
 * @category データ構造
 * @brief O(logN)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename M>
struct SegmentTree_Dual {
  using T = typename M::T;
  using E = typename M::E;

 private:
  const int height, n;
  vector<T> val;
  vector<E> laz;

 private:
  inline void eval(int k) {
    if (laz[k] == M::ei()) return;
    laz[(k << 1) | 0] = M::h(laz[(k << 1) | 0], laz[k]);
    laz[(k << 1) | 1] = M::h(laz[(k << 1) | 1], laz[k]);
    laz[k] = M::ei();
  }
  inline void thrust(int k) {
    for (int i = height; i; i--) eval(k >> i);
  }

 public:
  SegmentTree_Dual() {}
  SegmentTree_Dual(int n_, T v1 = T()) : SegmentTree_Dual(vector<T>(n_, v1)) {}
  SegmentTree_Dual(const vector<T>& v)
      : height(ceil(log2(v.size()))),
        n(1 << height),
        val(v),
        laz(n * 2, M::ei()) {}
  void update(int a, int b, E x) {
    thrust(a += n);
    thrust(b += n - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) laz[l] = M::h(laz[l], x), l++;
      if (r & 1) --r, laz[r] = M::h(laz[r], x);
    }
  }
  void set_val(int a, T x) {
    thrust(a += n);
    val[a] = x;
    laz[a] = M::ei();
  }
  T operator[](const int k) {
    thrust(k + n);
    if (laz[k + n] != M::ei()) {
      val[k] = M::g(val[k], laz[k + n]);
      laz[k + n] = M::ei();
    }
    return val[k];
  }
};