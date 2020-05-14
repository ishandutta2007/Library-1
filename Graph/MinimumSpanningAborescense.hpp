/**
 * @title 最小全域有向木
 * @category グラフ
 * @brief Chu-Liu/Edmonds
 * @brief O(m log n)
 * @brief 返り値:{木の重み,木の各ノードの親(根の親は根自身)}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/UnionFind.hpp"
#include "DataStructure/SkewHeap.hpp"
#undef call_from_test
#endif

template <typename T>
struct MinimumSpanningAborescense {
  struct Edge {
    int src, dst;
    T weight;
    Edge() {}
    Edge(int s, int d, int w) : src(s), dst(d), weight(w) {}
  };

 private:
  vector<Edge> edges;
  using Heap = SkewHeap<Edge, int>;
  int n;

 public:
  MinimumSpanningAborescense(int sz) : n(sz) {}
  void add_edge(int src, int dst, T weight) {
    edges.emplace_back(src, dst, weight);
  }
  pair<T, vector<int>> get_MSA(int root) {
    UnionFind uf(n);
    auto comp
        = [](const Edge &a, const Edge &b) { return a.weight > b.weight; };
    auto g = [](const Edge &a, const T &b) {
      return Edge(a.src, a.dst, a.weight + b);
    };
    vector<Heap> heap(n, Heap(comp, g));
    for (auto &e : edges) heap[e.dst].push(e);
    T score = 0;
    vector<int> seen(n, -1);
    seen[root] = root;
    vector<int> par(n, -1);
    par[root] = root;
    for (int s = 0; s < n; ++s) {
      vector<int> path;
      for (int u = s; seen[u] < 0;) {
        path.push_back(u);
        seen[u] = s;
        if (heap[u].empty()) return {-1, vector<int>()};
        Edge min_e = heap[u].top();
        par[min_e.dst] = min_e.src;
        score += min_e.weight;
        heap[u].add(-min_e.weight);
        heap[u].pop();
        int v = uf.root(min_e.src);
        if (seen[v] == s) {
          Heap new_heap(comp, g);
          while (1) {
            int w = path.back();
            path.pop_back();
            new_heap.merge(heap[w]);
            if (!uf.unite(v, w)) break;
          }
          heap[uf.root(v)] = new_heap;
          seen[uf.root(v)] = -1;
        }
        u = uf.root(v);
      }
    }
    return {score, par};
  }
};