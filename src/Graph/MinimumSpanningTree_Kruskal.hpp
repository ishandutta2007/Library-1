/**
 * @title 最小全域木(Kruskal)
 * @category グラフ
 *  O(E log V)
 *  返り値:{全域木のコスト総和,全域木に使用する辺}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/UnionFind.hpp"
#undef call_from_test
#endif

template <typename cost_t>
struct MinimumSpanningTree_Kruskal {
  struct Edge {
    int u, v, id;
    cost_t cost;
    bool operator<(const Edge &rhs) const { return this->cost < rhs.cost; }
    Edge() {}
    Edge(int s, int d, int i, int c) : u(s), v(d), id(i), cost(c) {}
  };

 private:
  int n;

 public:
  vector<Edge> edges;

 public:
  MinimumSpanningTree_Kruskal(int n) : n(n) {}
  void add_edge(int u, int v, cost_t cost) {
    edges.emplace_back(u, v, edges.size(), cost);
  }
  pair<cost_t, vector<Edge>> get_MST() {
    UnionFind uf(n);
    sort(begin(edges), end(edges));
    cost_t total = cost_t();
    vector<Edge> es;
    for (auto &e : edges)
      if (uf.unite(e.u, e.v)) {
        es.emplace_back(e);
        total += e.cost;
      }
    return {total, es};
  }
};
