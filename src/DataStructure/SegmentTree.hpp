/**
 * @title Segment-Tree
 * @category データ構造
 * O(logN)
 * 2冪にしていない(メモリ節約)
 */

// 2冪にするのやめて二次元array(10x10)をのせることに成功
// https://atcoder.jp/contests/arc027/tasks/arc027_4

// find_left, find_right verify
// https://atcoder.jp/contests/code-festival-2014-qualb/tasks/code_festival_qualB_d

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

 public:
  SegmentTree() {}
  SegmentTree(int n_) : n(n_), dat(n << 1, M::ti()) {}
  SegmentTree(int n_, T v) : SegmentTree(vector<T>(n_, v)) {}
  SegmentTree(const vector<T> &v) : n(v.size()), dat(n << 1, M::ti()) {
    for (int i = 0; i < (int)v.size(); i++) dat[i + n] = v[i];
    rebuild();
  }
  void set_val(int k, T x) {
    for (dat[k += n] = x; k >>= 1;)
      dat[k] = M::f(dat[(k << 1) | 0], dat[(k << 1) | 1]);
  }
  void unsafe_set(int k, T x) { dat[k + n] = x; }
  void rebuild() {
    for (int i = n - 1; i >= 1; i--)
      dat[i] = M::f(dat[i << 1 | 0], dat[i << 1 | 1]);
  }
  void clear() { fill(dat.begin(), dat.end(), M::ti()); }
  //[l,r)
  inline T query(int l, int r) {
    T vl = M::ti(), vr = M::ti();
    for (int a = l + n, b = r + n; a < b; a >>= 1, b >>= 1) {
      if (a & 1) vl = M::f(vl, dat[a++]);
      if (b & 1) vr = M::f(dat[--b], vr);
    }
    return M::f(vl, vr);
  }
  T operator[](const int &k) const { return dat[k + n]; }

  // max{ i : check(query(l,i+1)) = true}
  template <class C>
  int find_right(const C &check, int l = 0) {
    assert(check(M::ti()));
    if (l == n) return n;
    vector<int> idl, idr;
    for (int a = l + n, b = 2 * n; a < b; a >>= 1, b >>= 1) {
      if (a & 1) idl.push_back(a++);
      if (b & 1) idr.push_back(--b);
    }
    for (auto itr = idr.rbegin(); itr != idr.rend(); itr++) idl.push_back(*itr);
    T val = M::ti();
    for (int i : idl) {
      if (!check(M::f(val, dat[i]))) {
        while (i < n) {
          i = i << 1 | 0;
          if (check(M::f(val, dat[i]))) val = M::f(val, dat[i++]);
        }
        return i - n;
      }
      val = M::f(val, dat[i]);
    }
    return n;
  }
  //   min { i : check(query(i,r)) = true }
  template <class C>
  int find_left(const C &check, int r = -1) {
    if (r < 0) r = n;
    assert(check(M::ti()));
    if (r == 0) return 0;
    vector<int> idl, idr;
    for (int a = n, b = r + n; a < b; a >>= 1, b >>= 1) {
      if (a & 1) idl.push_back(a++);
      if (b & 1) idr.push_back(--b);
    }
    for (auto itr = idl.rbegin(); itr != idl.rend(); itr++) idr.push_back(*itr);
    T val = M::ti();
    for (int i : idr) {
      if (!check(M::f(dat[i], val))) {
        while (i < n) {
          i = i << 1 | 1;
          if (check(M::f(dat[i], val))) val = M::f(dat[i--], val);
        }
        return i + 1 - n;
      }
      val = M::f(dat[i], val);
    }
    return 0;
  }
};
