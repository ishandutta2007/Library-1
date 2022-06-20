#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2559"
#include <bits/stdc++.h>
#include "src/DataStructure/LinkCutTree.hpp"
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/MinimumSpanningTree_Kruskal.hpp"
using namespace std;

struct RchminQ {
  using T = long long;
  using E = long long;
  static void mapping(T &v, const E &f) {
    if (v > f) v = f;
  }
  static void composition(E &pre, const E &suf) {
    if (pre > suf) pre = suf;
  }
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
  LinkCutTree<RchminQ> lct(2 * n - 1, INT_MAX);
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
    lct.apply(e.u, e.v, e.cost);
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