/**
 * @title Segment-Tree
 * @category データ構造
 * @brief O(logN)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename M>
struct SegmentTree {
  using T = typename M::T;

 private:
  const int n;
  vector<T> dat;

 private:
  template <typename C>
  int find_subtree(int a, const C &check, T &cur, bool type) {
    while (a < n) {
      T nxt
          = type ? M::f(dat[2 * a + type], cur) : M::f(cur, dat[2 * a + type]);
      if (check(nxt))
        a = 2 * a + type;
      else
        cur = nxt, a = 2 * a + !type;
    }
    return a - n;
  }

 public:
  SegmentTree() {}
  SegmentTree(int n_, T v1 = M::ti()) : SegmentTree(vector<T>(n_, v1)) {}
  SegmentTree(const vector<T> &v)
      : n(1 << int(ceil(log2(v.size())))), dat(n << 1, M::ti()) {
    for (int i = 0; i < (int)v.size(); i++) dat[i + n] = v[i];
    for (int i = n - 1; i >= 1; i--)
      dat[i] = M::f(dat[i << 1 | 0], dat[i << 1 | 1]);
  }
  void set_val(int k, T x) {
    for (dat[k += n] = x; k >>= 1;)
      dat[k] = M::f(dat[(k << 1) | 0], dat[(k << 1) | 1]);
  }
  //[a,b)
  T query(int a, int b) {
    T vl = M::ti(), vr = M::ti();
    for (int l = a + n, r = b + n; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vl = M::f(vl, dat[l++]);
      if (r & 1) vr = M::f(dat[--r], vr);
    }
    return M::f(vl, vr);
  }
  T operator[](const int &k) const { return dat[k + n]; }
  // min { i : check(query(a,i+1)) = true }
  template <typename C>
  int find_first(const C &check, int a = 0) {
    T vl = M::ti();
    if (a <= 0) {
      if (check(M::f(vl, dat[1]))) return find_subtree(1, check, vl, false);
      return -1;
    }
    int b = n;
    for (a += n, b += n; a < b; a >>= 1, b >>= 1)
      if (a & 1) {
        T nxt = M::f(vl, dat[a]);
        if (check(nxt)) return find_subtree(a, check, vl, false);
        vl = nxt;
        ++a;
      }
    return -1;
  }
  // max { i : check(query(i,b)) = true }
  template <typename C>
  int find_last(const C &check, int b = n) {
    T vr = M::ti();
    if (b >= n) {
      if (check(M::f(dat[1], vr))) return find_subtree(1, check, vr, true);
      return -1;
    }
    int a = 0;
    for (a += n, b += n; a < b; a >>= 1, b >>= 1)
      if (b & 1) {
        T nxt = M::f(dat[--b], vr);
        if (check(nxt)) return find_subtree(b, check, vr, true);
        vr = nxt;
      }
    return -1;
  }
};