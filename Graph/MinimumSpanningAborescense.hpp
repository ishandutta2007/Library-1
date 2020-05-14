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
    int src, dst, id;
    T weight;
    Edge() {}
    Edge(int s, int d, int i, int w) : src(s), dst(d), id(i), weight(w) {}
  };

 private:
  vector<Edge> edges;
  using Heap = SkewHeap<Edge, int>;
  int n;

 public:
  MinimumSpanningAborescense(int sz) : n(sz) {}
  void add_edge(int src, int dst, T weight) {
    edges.emplace_back(src, dst, edges.size(), weight);
  }
  pair<T, vector<int>> get_MSA(int root) {
    UnionFind uf(n);
    auto comp
        = [](const Edge &a, const Edge &b) { return a.weight > b.weight; };
    auto g = [](const Edge &a, const T &b) {
      return Edge(a.src, a.dst, a.id, a.weight + b);
    };
    vector<Heap> heap(n, Heap(comp, g));
    for (auto &e : edges) heap[e.dst].push(e);
    T score = 0;
    int m = edges.size();
    vector<int> seen(n, -1), paredge(m), ei, leaf(n, -1), par(n), usede(m);
    seen[root] = root;
    for (int s = 0; s < n; ++s) {
      vector<int> path, ch;
      int cyc = 0;
      for (int u = s; seen[u] < 0;) {
        path.push_back(u);
        seen[u] = s;
        if (heap[u].empty()) return {-1, vector<int>()};
        Edge min_e = heap[u].top();
        score += min_e.weight;
        heap[u].add(-min_e.weight);
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
          Heap new_heap(comp, g);
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
    par[root] = root;
    for (auto i : ei) {
      if (usede[i]) continue;
      par[edges[i].dst] = edges[i].src;
      int x = leaf[edges[i].dst];
      while (x != i) {
        usede[x] = 1;
        x = paredge[x];
      }
    }
    return {score, par};
  }
};