#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2450"

// Do use segment tree

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/LinkCutTree_MonoidLazy.hpp"
#undef call_from_test

struct RupdQ_RmaxsumQ {
  static constexpr long long INF = INT_MAX;
  struct T {
    long long sum, max, lmax, rmax;
    int size;
  };
  using E = long long;
  static T ti() { return {0ll, -INF, -INF, -INF, 0}; }
  static E ei() { return INF; }
  static T f(const T &a, const T &b) {
    T ret;
    ret.sum = a.sum + b.sum;
    ret.max = max({a.max, b.max, a.rmax + b.lmax});
    ret.lmax = max(a.lmax, a.sum + b.lmax);
    ret.rmax = max(a.rmax + b.sum, b.rmax);
    ret.size = a.size + b.size;
    return ret;
  }
  static T g(const T &l, const E &r) {
    if (r == ei()) return l;
    return {r * l.size, max(r * l.size, r), max(r * l.size, r),
            max(r * l.size, r), l.size};
  }
  static E h(const E &l, const E &r) { return r == ei() ? l : r; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  LinkCutTree_MonoidLazy<RupdQ_RmaxsumQ> lct(N);
  for (int i = 0; i < N; i++) {
    long long w;
    cin >> w;
    lct.set_val(i, {w, w, w, w, 1});
  }
  for (int i = 0; i < N - 1; i++) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    lct.link(u, v);
  }
  while (Q--) {
    long long t, a, b, c;
    cin >> t >> a >> b >> c;
    a--, b--;
    if (t == 1) {
      lct.update(a, b, c);
    } else {
      cout << lct.query(a, b).max << endl;
    }
  }
  return 0;
}
