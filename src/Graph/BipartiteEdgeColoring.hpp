#pragma once
#include <array>
#include <queue>
#include <numeric>
#include "src/DataStructure/UnionFind.hpp"
class BipartiteEdgeColoring {
 std::vector<std::array<int, 2>> es_;
 const int n[2];
public:
 BipartiteEdgeColoring(int L, int R): n{L, R} {}
 void add_edge(int l, int r) { es_.push_back({l, r}); }
 std::vector<int> edge_coloring() {
  auto es= es_;
  const int m= es.size();
  std::vector<int> color(m, -1), deg[2]= {std::vector<int>(n[0]), std::vector<int>(n[1])};
  for (auto [l, r]: es) ++deg[0][l], ++deg[1][r];
  const int D= std::max(*std::max_element(deg[0].begin(), deg[0].end()), *std::max_element(deg[1].begin(), deg[1].end()));
  UnionFind uf[2]= {UnionFind(n[0]), UnionFind(n[1])};
  int cnt[2], col= 0;
  for (int s= 2; s--;) {
   std::priority_queue<std::pair<int, int>> pq;
   for (int i= n[s]; i--;) pq.emplace(-deg[s][i], i);
   for (cnt[s]= pq.size(); pq.size() > 1;) {
    auto [as, av]= pq.top();
    pq.pop();
    auto [bs, bv]= pq.top();
    pq.pop();
    if (int sum= as + bs; -sum <= D) uf[s].unite(av, bv), pq.emplace(sum, av), --cnt[s];
    else break;
   }
  }
  std::vector<int> id[2]= {std::vector<int>(n[0], -1), std::vector<int>(n[1], -1)};
  for (int s= 2; s--;)
   for (int i= n[s], j= 0; i--;)
    if (uf[s].root(i) == i) id[s][i]= j++;
  const int n= std::max(cnt[0], cnt[1]);
  deg[0].assign(n, 0), deg[1].assign(n, 0), es.reserve(n * D);
  for (auto &e: es) ++deg[0][e[0]= id[0][uf[0].root(e[0])]], ++deg[1][e[1]= id[1][uf[1].root(e[1])]], e[1]+= n;
  for (int v= 0, w= 0; v < n; ++v) {
   while (deg[0][v] < D) {
    while (w < n && deg[1][w] == D) ++w;
    int x= std::min(D - deg[1][w], D - deg[0][v]);
    for (int k= x; k--;) es.push_back({v, w + n});
    deg[0][v]+= x, deg[1][w]+= x;
   }
  }
  auto rec= [&](auto rec, int d, const std::vector<int> &idx) {
   if (!d) return;
   if (d == 1) {
    for (int e: idx)
     if (e < m) color[e]= col;
    ++col;
   } else if (d & 1) {
    std::vector<int> mate(n + n, -1), g(idx.size()), pos(n + 1), pre, que(n), rt;
    for (int e: idx) ++pos[es[e][0]];
    for (int i= 0; i < n; ++i) pos[i + 1]+= pos[i];
    for (int e: idx) {
     auto [l, r]= es[e];
     g[--pos[l]]= r;
    }
    for (bool upd= true; upd;) {
     upd= false, rt.assign(n, -1), pre.assign(n, -1);
     int t= 0;
     for (int l= n; l--;)
      if (mate[l] == -1) que[t++]= rt[l]= pre[l]= l;
     for (int i= 0; i < t; ++i)
      if (int v= que[i]; mate[rt[v]] == -1)
       for (int j= pos[v], u, w; j < pos[v + 1]; ++j) {
        if (w= mate[u= g[j]]; w == -1) {
         for (upd= true; u != -1; v= pre[v]) mate[u]= v, std::swap(mate[v], u);
         break;
        }
        if (pre[w] == -1) rt[que[t++]= w]= rt[pre[w]= v];
       }
    }
    rt.clear(), rt.reserve(idx.size());
    for (int e: idx) {
     auto [l, r]= es[e];
     if (mate[l] == r) {
      if (mate[l]= mate[r]= -1; e < m) color[e]= col;
     } else rt.push_back(e);
    }
    ++col, rec(rec, d - 1, rt);
   } else {
    const int mm= idx.size();
    std::vector<std::array<int, 2>> g(mm * 2), st;
    std::vector<int> pos(n + n + 1), circuit, half1(mm / 2), half2(mm / 2);
    std::vector<int8_t> used1(n + n), used2(mm);
    for (int e: idx) {
     auto [l, r]= es[e];
     ++pos[l], ++pos[r];
    }
    std::partial_sum(pos.begin(), pos.end(), pos.begin()), st.reserve(mm + 1), circuit.reserve(mm + 1);
    for (int i= mm; i--;) {
     auto [l, r]= es[idx[i]];
     g[--pos[l]]= {r, i}, g[--pos[r]]= {l, i};
    }
    std::vector<int> it(pos.begin(), pos.begin() + n + n);
    for (int v= n + n; v--;)
     if (!used1[v]) {
      for (st.clear(), st.push_back({v, -1}); st.size();) {
       auto [p, e]= st.back();
       used1[p]= true;
       if (it[p] == pos[p + 1]) circuit.push_back(e), st.pop_back();
       else if (const auto &a= g[it[p]++]; !used2[a[1]]) used2[a[1]]= true, st.push_back(a);
      }
      circuit.pop_back();
     }
    for (int i= 0; i < mm; i+= 2) half1[i / 2]= idx[circuit[i]], half2[i / 2]= idx[circuit[i + 1]];
    rec(rec, d / 2, half1), rec(rec, d / 2, half2);
   }
  };
  std::vector<int> idx(n * D);
  return iota(idx.begin(), idx.end(), 0), rec(rec, D, idx), color;
 }
};