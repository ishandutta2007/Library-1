#define PROBLEM "https://atcoder.jp/contests/abc133/tasks/abc133_f"

// 永続配列(at) の verify

#include <bits/stdc++.h>
#include "src/DataStructure/DynamicSegmentTree.hpp"
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  LinkCutTree lct(N);
  vector<tuple<int, int, int>> adj[N];
  for (int i = 0; i < N - 1; i++) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    lct.link(--a, --b);
    adj[a].emplace_back(b, c, d);
    adj[b].emplace_back(a, c, d);
  }
  using PerArr = DynamicSegmentTree<int, true, 17>;
  PerArr Arr1[N], Arr2[N];
  int sum[N] = {0};
  auto dfs = [&adj, &Arr1, &Arr2, &sum](auto f, int v, int p) -> void {
    for (auto [u, c, d] : adj[v])
      if (u != p) {
        Arr1[u] = Arr1[v];
        if (Arr1[u].is_null(c))
          Arr1[u][c] = 1;
        else
          Arr1[u][c] += 1;
        Arr2[u] = Arr2[v];
        if (Arr2[u].is_null(c))
          Arr2[u][c] = d;
        else
          Arr2[u][c] += d;
        sum[u] = sum[v] + d;
        f(f, u, v);
      }
  };
  dfs(dfs, 0, -1);
  lct.evert(0);
  for (int i = 0; i < Q; i++) {
    int x, y, u, v;
    cin >> x >> y >> u >> v;
    int lca = lct.lca(--u, --v);
    cout << sum[u] + sum[v] - 2 * sum[lca] +
                y * (Arr1[u].get(x) + Arr1[v].get(x) - 2 * Arr1[lca].get(x)) -
                (Arr2[u].get(x) + Arr2[v].get(x) - 2 * Arr2[lca].get(x))
         << '\n';
  }
  return 0;
}