#define PROBLEM "https://yukicoder.me/problems/no/529"
#include <bits/stdc++.h>
#include "src/Graph/TwoEdgeConnectedComponents.hpp"
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;

struct RmaxQ {
  using T = pair<long long, int>;
  static T op(const T &vl, const T &vr) {
    return vl.first > vr.first ? vl : vr;
  }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M, Q;
  cin >> N >> M >> Q;
  pair<int, int> edges[M];
  for (int i = 0; i < M; i++) {
    cin >> edges[i].first >> edges[i].second;
    edges[i].first--, edges[i].second--;
  }
  TwoEdgeConnectedComponents graph(N);
  for (int i = 0; i < M; i++) {
    auto [A, B] = edges[i];
    graph.add_edge(A, B);
  }
  auto res = graph.get_2ECC();
  int n = res.first.size();
  auto index = res.second;
  LinkCutTree<RmaxQ> lct(n);
  for (int i = 0; i < M; i++) {
    auto [A, B] = edges[i];
    int u = index[A], v = index[B];
    if (u == v) continue;
    lct.link(u, v);
  }
  priority_queue<long long> pq[n];
  for (int i = 0; i < n; i++) pq[i].push(-1);
  while (Q--) {
    int op, x, y;
    cin >> op >> x >> y;
    if (op == 1) {
      int u = index[--x];
      pq[u].push(y);
      lct.set_val(u, make_pair(pq[u].top(), u));
    } else {
      int u = index[--x], v = index[--y];
      auto [ans, i] = lct.fold(u, v);
      cout << ans << endl;
      if (ans != -1) {
        pq[i].pop();
        lct.set_val(i, make_pair(pq[i].top(), i));
      }
    }
  }
  return 0;
}