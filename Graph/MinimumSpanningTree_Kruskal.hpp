/**
 * @title 最小全域木(Kruskal)
 * @category グラフ
 * @brief O(m log n)
 * @brief 返り値:{全域木のコスト総和,全域木に使用する辺}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/UnionFind.hpp"
#undef call_from_test
#endif

template <typename cost_t>
struct MinimumSpanningTree_Kruskal {
  struct Edge {
    int src, dst, id;
    cost_t cost;
    bool operator<(const Edge &rhs) const { return this->cost < rhs.cost; }
    Edge() {}
    Edge(int s, int d, int i, int c) : src(s), dst(d), id(i), cost(c) {}
  };

 private:
  int n;
  vector<Edge> edges;

 public:
  MinimumSpanningTree_Kruskal(int n) : n(n) {}
  void add_edge(int src, int dst, cost_t cost) {
    edges.emplace_back(src, dst, edges.size(), cost);
  }
  pair<cost_t, vector<Edge>> get_MST() {
    UnionFind uf(n);
    sort(begin(edges), end(edges));
    cost_t total = cost_t();
    vector<Edge> es;
    for (auto &e : edges)
      if (uf.unite(e.src, e.dst)) {
        es.emplace_back(e);
        total += e.cost;
      }
    return {total, es};
  }
};