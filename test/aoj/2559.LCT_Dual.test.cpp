#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2559"
#include <bits/stdc++.h>
#include "src/DataStructure/LinkCutTree_MonoidDual.hpp"
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/MinimumSpanningTree_Kruskal.hpp"
using namespace std;

struct RchminQ {
  using T = long long;
  using E = long long;
  static E ei() { return INT_MAX; }
  static T g(const T &l, const E &r) { return min(l, r); }
  static E h(const E &l, const E &r) { return min(l, r); }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int n, m;
  cin >> n >> m;
  MinimumSpanningTree_Kruskal<long long> graph(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    long long w;
    cin >> a >> b >> w;
    a--, b--;
    graph.add_edge(a, b, w);
  }
  auto res = graph.get_MST();
  LinkCutTree_MonoidDual<RchminQ> lct(2 * n - 1, INT_MAX);
  int id[m];
  fill(id, id + m, -1);
  int num = n;
  for (auto e : res.second) {
    id[e.id] = num++;
    lct.link(e.u, id[e.id]);
    lct.link(id[e.id], e.v);
  }
  for (auto e : graph.edges) {
    if (id[e.id] != -1) continue;
    lct.update(e.u, e.v, e.cost);
  }
  long long ans[m];
  for (auto e : graph.edges) {
    if (id[e.id] == -1) {
      ans[e.id] = res.first;
    } else {
      long long tmp = lct[id[e.id]];
      ans[e.id] = tmp == INT_MAX ? -1 : res.first - e.cost + tmp;
    }
  }
  for (auto a : ans) cout << a << endl;
  return 0;
}