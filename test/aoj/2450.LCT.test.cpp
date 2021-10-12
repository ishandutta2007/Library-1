#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Summer/2450"
// Do use segment tree
#include <bits/stdc++.h>
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;

struct RupdQ_RmaxsumQ {
  struct T {
    long long sum, max, lmax, rmax;
    int size;
  };
  using E = long long;
  static T op(const T &a, const T &b) {
    T ret;
    ret.sum = a.sum + b.sum;
    ret.max = max({a.max, b.max, a.rmax + b.lmax});
    ret.lmax = max(a.lmax, a.sum + b.lmax);
    ret.rmax = max(a.rmax + b.sum, b.rmax);
    ret.size = a.size + b.size;
    return ret;
  }
  static T mapping(const T &l, const E &r) {
    return {r * l.size, max(r * l.size, r), max(r * l.size, r),
            max(r * l.size, r), l.size};
  }
  static E composition(const E &l, const E &r) { return r; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  LinkCutTree<RupdQ_RmaxsumQ> lct(N);
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
      lct.apply(a, b, c);
    } else {
      cout << lct.fold(a, b).max << endl;
    }
  }
  return 0;
}
