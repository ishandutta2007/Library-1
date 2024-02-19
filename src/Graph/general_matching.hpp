#pragma once
#include <cassert>
#include "src/Graph/Graph.hpp"
// {matching edge ids, partner(-1 if unmatched)}
std::pair<std::vector<int>, std::vector<int>> general_matching(const Graph &g, const std::vector<int> &partner= {}) {
 auto adj= g.adjacency_vertex(0);
 const int n= adj.size();
 std::vector<int> q, z(n), p(n), m(partner);
 if (m.empty()) m.assign(n, -1);
 assert((int)m.size() == n);
 std::vector<Edge> fs(n);
 auto rematch= [&](auto &&rc, int u, int v) -> void {
  int w= m[u];
  if (m[u]= v; w == -1 || m[w] != u) return;
  if (auto [x, y]= fs[u]; y == -1) rc(rc, m[w]= x, w);
  else rc(rc, x, y), rc(rc, y, x);
 };
 int t= 1;
 auto f= [&](auto &&rc, int x) -> int { return z[x] != t || p[x] == -1 ? x : (p[x]= rc(rc, p[x])); };
 auto check= [&](int r) {
  q.clear(), q.push_back(r), fs[r]= {-1, -1}, z[r]= t, p[r]= -1;
  for (int i= 0; i < (int)q.size(); ++i) {
   int x= q[i];
   for (int y: adj[x]) {
    if (y == r) continue;
    if (m[y] == -1) return rematch(rematch, m[y]= x, y), true;
    if (z[y] == t) {
     int u= f(f, x), v= f(f, y), w= r;
     if (u == v) continue;
     for (; u != r || v != r; fs[u]= {x, y}, u= f(f, fs[m[u]].first)) {
      if (v != r) std::swap(u, v);
      if (fs[u].first == x && fs[u].second == y) {
       w= u;
       break;
      }
     }
     for (int a: {f(f, x), f(f, y)})
      for (; a != w; a= f(f, fs[m[a]].first)) z[a]= t, p[a]= w, q.push_back(a);
    } else if (z[m[y]] != t) fs[y]= {-1, -1}, fs[m[y]]= {x, -1}, z[m[y]]= t, p[m[y]]= y, q.push_back(m[y]);
   }
  }
  return false;
 };
 for (int r= n; r--;)
  if (m[r] == -1) t+= check(r);
 q.clear();
 for (int i= 0, e= g.edge_size(); i < e; ++i)
  if (auto [u, v]= g[i]; m[u] == v && z[u] >= 0) q.push_back(i), z[u]= z[v]= -1;
 return {q, m};
}