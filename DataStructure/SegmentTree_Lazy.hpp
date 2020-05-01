/**
 * @title Segment-Tree(遅延伝搬)
 * @category データ構造
 * @brief O(logN)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename M>
class SegmentTree_Lazy {
 private:
  using T = typename M::T;
  using E = typename M::E;
  int height, n;
  vector<T> dat;
  vector<E> laz;
  inline T reflect(int k) {
    return laz[k] == M::ei() ? dat[k] : M::g(dat[k], laz[k]);
  }
  inline void eval(int k) {
    if (laz[k] == M::ei()) return;
    laz[(k << 1) | 0] = M::h(laz[(k << 1) | 0], laz[k]);
    laz[(k << 1) | 1] = M::h(laz[(k << 1) | 1], laz[k]);
    dat[k] = reflect(k);
    laz[k] = M::ei();
  }
  inline void thrust(int k) {
    for (int i = height; i; i--) eval(k >> i);
  }
  inline void recalc(int k) {
    while (k >>= 1) dat[k] = M::f(reflect((k << 1) | 0), reflect((k << 1) | 1));
  }
  template <typename C>
  int find_subtree(int a, const C& check, T& cur, bool type) {
    while (a < n) {
      eval(a);
      T nxt = type ? M::f(reflect(2 * a + type), cur)
                   : M::f(cur, reflect(2 * a + type));
      if (check(nxt))
        a = 2 * a + type;
      else
        cur = nxt, a = 2 * a + !type;
    }
    return a - n;
  }

 public:
  SegmentTree_Lazy() {}
  SegmentTree_Lazy(int n_, T v1 = M::ti())
      : SegmentTree_Lazy(vector<T>(n_, v1)) {}
  SegmentTree_Lazy(const vector<T>& v)
      : height(ceil(log2(v.size()))),
        n(1 << height),
        dat(n * 2, M::ti()),
        laz(n * 2, M::ei()) {
    for (int i = 0; i < (int)v.size(); i++) dat[i + n] = v[i];
    for (int i = n - 1; i >= 1; i--)
      dat[i] = M::f(dat[i << 1 | 0], dat[i << 1 | 1]);
  }
  void update(int a, int b, E x) {
    thrust(a += n);
    thrust(b += n - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) laz[l] = M::h(laz[l], x), l++;
      if (r & 1) --r, laz[r] = M::h(laz[r], x);
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
  T query(int a, int b) {
    thrust(a += n);
    thrust(b += n - 1);
    T vl = M::ti(), vr = M::ti();
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vl = M::f(vl, reflect(l++));
      if (r & 1) vr = M::f(reflect(--r), vr);
    }
    return M::f(vl, vr);
  }
  T operator[](const int k) { return query(k, k + 1); }
  // min { i : check(query(a,i)) = true }
  template <typename C>
  int find_first(int a, const C& check) {
    T vl = M::ti();
    if (a <= 0) {
      if (check(M::f(vl, reflect(1)))) return find_subtree(1, check, vl, false);
      return -1;
    }
    thrust(a + n);
    int b = n;
    for (a += n, b += n; a < b; a >>= 1, b >>= 1)
      if (a & 1) {
        T nxt = M::f(vl, reflect(a));
        if (check(nxt)) return find_subtree(a, check, vl, false);
        vl = nxt;
        ++a;
      }
    return -1;
  }
  // max { i : check(query(i,b)) = true }
  template <typename C>
  int find_last(int b, const C& check) {
    T vr = M::ti();
    if (b >= n) {
      if (check(M::f(reflect(1), vr))) return find_subtree(1, check, vr, true);
      return -1;
    }
    thrust(b + n - 1);
    int a = 0;
    for (a += n, b += n; a < b; a >>= 1, b >>= 1)
      if (b & 1) {
        T nxt = M::f(reflect(--b), vr);
        if (check(nxt)) return find_subtree(b, check, vr, true);
        vr = nxt;
      }
    return -1;
  }
};