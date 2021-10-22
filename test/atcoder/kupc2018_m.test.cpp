#define PROBLEM "https://atcoder.jp/contests/kupc2018/tasks/kupc2018_m"

// パトリシア木 と fold + xor の verify

#include <bits/stdc++.h>
#include "src/DataStructure/DynamicSegmentTree.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
using namespace std;

struct RsumQ {
  using T = long long;
  static T ti() { return 0; }
  static T op(T vl, T vr) { return vl + vr; }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Seg = DynamicSegmentTree<RsumQ, false, 30, true>;
  int N;
  cin >> N;
  HeavyLightDecomposition hld(N);
  for (int i = 0; i < N - 1; i++) {
    int a, b;
    cin >> a >> b;
    hld.add_edge(--a, --b);
  }
  hld.build(0);
  int m = 1;
  while (m < N) m <<= 1;
  Seg ET[2 * m];
  int Q;
  cin >> Q;
  int root = 0;
  for (int i = 0; i < Q; i++) {
    int op, v;
    cin >> op >> v;
    v--;
    if (op == 1) {
      int x;
      long long k;
      cin >> x >> k;
      int l, r;
      if (root == v) {
        ET[1].set(x, ET[1].get(x) + k);
      } else if (hld.in[v] < hld.in[root] &&
                 hld.in[root] < hld.out[hld.in[v]]) {
        int u = root;
        while (hld.head[v] != hld.head[u]) {
          if (v == hld.par[hld.head[u]]) {
            l = hld.in[hld.head[u]];
            r = hld.out[hld.in[hld.head[u]]];
            break;
          }
          u = hld.par[hld.head[u]];
        }
        if (hld.head[v] == hld.head[u]) {
          l = hld.in[v] + 1;
          r = hld.out[hld.in[v] + 1];
        }
        for (l += m, r += m; l < r; l >>= 1, r >>= 1) {
          if (l & 1) ET[l].set(x, ET[l].get(x) - k), l++;
          if (r & 1) --r, ET[r].set(x, ET[r].get(x) - k);
        }
        ET[1].set(x, ET[1].get(x) + k);
      } else {
        l = hld.in[v];
        r = hld.out[hld.in[v]];
        for (l += m, r += m; l < r; l >>= 1, r >>= 1) {
          if (l & 1) ET[l].set(x, ET[l].get(x) + k), l++;
          if (r & 1) --r, ET[r].set(x, ET[r].get(x) + k);
        }
      }
    } else if (op == 2) {
      int y, z;
      long long ans = 0;
      cin >> y >> z;
      for (int i = hld.in[v] + m; i; i >>= 1) {
        ans += ET[i].fold(0, z + 1, y);
      }
      cout << ans << '\n';
    } else {
      root = v;
    }
  }
  return 0;
}