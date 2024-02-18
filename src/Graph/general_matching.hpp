#pragma once
#include <cassert>
#include "src/Graph/Graph.hpp"
// {matching, partner(-1 if unmatched)}
std::pair<std::vector<int>, std::vector<int>> general_matching(const Graph &g, const std::vector<int> &partner= {}) {
 auto adj= g.adjacency_vertex(0);
 const int n= adj.size();
 std::vector<int> q(n), id(n, -1), p(n), m(partner);
 if (m.empty()) m.assign(n, -1);
 assert((int)m.size() == n);
 std::vector<Edge> fs(n);
 auto rematch= [&](auto self, int u, int v) -> void {
  int w;
  if (w= m[u], m[u]= v; w == -1 || m[w] != u) return;
  if (auto [x, y]= fs[u]; y == -1) self(self, m[w]= x, w);
  else self(self, x, y), self(self, y, x);
 };
 int t= 0;
 auto f= [&](auto self, int x) -> int { return id[x] != t || p[x] == -1 ? x : (p[x]= self(self, p[x])); };
 auto check= [&](int rt) {
  fs[rt]= {-1, -1}, id[rt]= t, p[q[0]= rt]= -1;
  for (int i= 0, s= 1; i < s; ++i) {
   int x= q[i];
   for (int y: adj[x]) {
    if (y == rt) continue;
    if (m[y] == -1) return rematch(rematch, m[y]= x, y), true;
    if (id[y] == t) {
     int u= f(f, x), v= f(f, y), w= rt;
     if (u == v) continue;
     for (; u != rt || v != rt; fs[u]= {x, y}, u= f(f, fs[m[u]].first)) {
      if (v != rt) std::swap(u, v);
      if (fs[u].first == x && fs[u].second == y) {
       w= u;
       break;
      }
     }
     for (int a= u; a != w; a= f(f, fs[m[a]].first)) id[a]= t, p[a]= w, q[s++]= a;
     for (int b= v; b != w; b= f(f, fs[m[b]].first)) id[b]= t, p[b]= w, q[s++]= b;
    } else if (id[m[y]] != t) fs[y]= {-1, -1}, fs[m[y]]= {x, -1}, id[m[y]]= t, p[m[y]]= y, q[s++]= m[y];
   }
  }
  return false;
 };
 for (int rt= n; rt--;)
  if (m[rt] == -1) t+= check(rt);
 p.clear();
 for (int i= 0, e= g.edge_size(); i < e; ++i)
  if (auto [u, v]= g[i]; m[u] == v && q[u] >= 0) p.push_back(i), q[u]= q[v]= -1;
 return {p, m};
}