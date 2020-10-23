#pragma once
#include <bits/stdc++.h>
#include "src/DataStructure/UnionFind.hpp"
/**
 * @title 最小全域木(Kruskal)
 * @category グラフ
 *  O(E log V)
 *  返り値:{全域木のコスト総和,全域木に使用する辺}
 */

// BEGIN CUT HERE

template <typename cost_t>
struct MinimumSpanningTree_Kruskal {
  struct Edge {
    int u, v, id;
    cost_t cost;
    bool operator<(const Edge &rhs) const { return this->cost < rhs.cost; }
  };

 private:
  int n;

 public:
  std::vector<Edge> edges;

 public:
  MinimumSpanningTree_Kruskal(int n) : n(n) {}
  void add_edge(int u, int v, cost_t cost) {
    edges.emplace_back(Edge{u, v, (int)edges.size(), cost});
  }
  std::pair<cost_t, std::vector<Edge>> get_MST() {
    UnionFind uf(n);
    sort(begin(edges), end(edges));
    cost_t total = cost_t();
    std::vector<Edge> es;
    for (auto &e : edges)
      if (uf.unite(e.u, e.v)) {
        es.emplace_back(e);
        total += e.cost;
      }
    return {total, es};
  }
};