#pragma once
#include <bits/stdc++.h>
#include "src/DataStructure/UnionFind.hpp"
#include "src/DataStructure/SkewHeap.hpp"
/**
 * @title 最小全域有向木
 * @category グラフ
 *  Chu-Liu/Edmonds
 *  O(E log V)
 *  返り値:{全域木のコスト総和,全域木に使用する辺}
 */

// BEGIN CUT HERE

template <typename cost_t>
class MinimumSpanningAborescense {
  struct Edge {
    std::size_t src, dst, id;
    cost_t cost;
    bool operator>(const Edge &r) const { return this->cost > r.cost; }
  };
  struct Op_Edge_add {
    using E = cost_t;
    static Edge mapping(const Edge &l, const E &r) {
      return Edge{l.src, l.dst, l.id, l.cost + r};
    }
    static E composition(const E &l, const E &r) { return l + r; }
  };
  using Heap = SkewHeap<Edge, std::greater<Edge>, Op_Edge_add>;
  std::size_t n;
  std::vector<Edge> edges;

 public:
  MinimumSpanningAborescense(std::size_t n) : n(n) {}
  void add_edge(std::size_t src, std::size_t dst, cost_t cost) {
    edges.emplace_back(Edge{src, dst, edges.size(), cost});
  }
  std::pair<cost_t, std::vector<Edge>> get_MSA(int root) {
    UnionFind uf(n);
    std::vector<Heap> heap(n);
    for (auto &e : edges) heap[e.dst].push(e);
    cost_t score = 0;
    std::size_t m = edges.size(), s = 0, u = s, cyc = 0, v;
    std::vector<int> seen(n, -1), paredge(m), ei, leaf(n, -1), par(n), usede(m);
    for (seen[root] = root; s < n; u = ++s, cyc = 0)
      for (std::vector<int> path, ch; seen[u] < 0; u = uf.root(v)) {
        path.push_back(u), seen[u] = s;
        if (heap[u].empty()) return {-1, std::vector<Edge>()};
        Edge min_e = heap[u].pop();
        score += min_e.cost, heap[u].apply(-min_e.cost), ei.push_back(min_e.id);
        if (leaf[min_e.dst] == -1) leaf[min_e.dst] = min_e.id;
        for (; cyc; cyc--) paredge[ch.back()] = min_e.id, ch.pop_back();
        ch.push_back(min_e.id);
        if (seen[v = uf.root(min_e.src)] != s) continue;
        Heap new_heap;
        for (int w; 1;) {
          new_heap += heap[w = path.back()];
          path.pop_back(), cyc++;
          if (!uf.unite(v, w)) break;
        }
        heap[uf.root(v)] = new_heap, seen[uf.root(v)] = -1;
      }
    std::vector<Edge> es;
    for (auto it = ei.rbegin(); it != ei.rend(); it++) {
      if (usede[*it]) continue;
      es.emplace_back(edges[*it]);
      for (int x = leaf[edges[*it].dst]; x != *it; x = paredge[x]) usede[x] = 1;
    }
    return {score, es};
  }
};