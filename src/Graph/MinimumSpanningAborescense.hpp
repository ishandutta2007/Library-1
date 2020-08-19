/**
 * @title 最小全域有向木
 * @category グラフ
 *  Chu-Liu/Edmonds
 *  O(E log V)
 *  返り値:{全域木のコスト総和,全域木に使用する辺}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/UnionFind.hpp"
#include "src/DataStructure/SkewHeap.hpp"
#undef call_from_test
#endif

template <typename cost_t>
struct MinimumSpanningAborescense {
  struct Edge {
    int src, dst, id;
    cost_t cost;
    bool operator>(const Edge &r) const { return this->cost > r.cost; }
  };
  struct Op_Edge_add {
    using E = cost_t;
    static E ei() { return 0; }
    static Edge g(const Edge &l, const E &r) {
      return Edge({l.src, l.dst, l.id, l.cost + r});
    }
    static E h(const E &l, const E &r) { return l + r; }
  };
  using Heap = SkewHeap<Edge, greater<Edge>, Op_Edge_add>;

 private:
  int n;

 public:
  vector<Edge> edges;

 public:
  MinimumSpanningAborescense(int n) : n(n) {}
  void add_edge(int src, int dst, cost_t cost) {
    edges.emplace_back(Edge{src, dst, (int)edges.size(), cost});
  }
  pair<cost_t, vector<Edge>> get_MSA(int root) {
    UnionFind uf(n);
    vector<Heap> heap(n);
    for (auto &e : edges) heap[e.dst].push(e);
    cost_t score = 0;
    int m = edges.size();
    vector<int> seen(n, -1), paredge(m), ei, leaf(n, -1), par(n), usede(m);
    seen[root] = root;
    for (int s = 0; s < n; ++s) {
      vector<int> path, ch;
      int cyc = 0;
      for (int u = s; seen[u] < 0;) {
        path.push_back(u);
        seen[u] = s;
        if (heap[u].empty()) return {-1, vector<Edge>()};
        Edge min_e = heap[u].top();
        score += min_e.cost;
        heap[u].add(-min_e.cost);
        heap[u].pop();
        ei.push_back(min_e.id);
        if (leaf[min_e.dst] == -1) leaf[min_e.dst] = min_e.id;
        while (cyc) {
          paredge[ch.back()] = min_e.id;
          ch.pop_back();
          cyc--;
        }
        ch.push_back(min_e.id);
        int v = uf.root(min_e.src);
        if (seen[v] == s) {
          Heap new_heap;
          while (1) {
            int w = path.back();
            path.pop_back();
            new_heap.merge(heap[w]);
            cyc++;
            if (!uf.unite(v, w)) break;
          }
          heap[uf.root(v)] = new_heap;
          seen[uf.root(v)] = -1;
        }
        u = uf.root(v);
      }
    }
    reverse(ei.begin(), ei.end());
    vector<Edge> es;
    for (auto i : ei) {
      if (usede[i]) continue;
      es.emplace_back(edges[i]);
      int x = leaf[edges[i].dst];
      while (x != i) {
        usede[x] = 1;
        x = paredge[x];
      }
    }
    return {score, es};
  }
};