#pragma once
#include "src/Graph/Graph.hpp"
std::vector<Edge> general_matching(const CSRArray<int> &adj) {
 const int n= adj.size();
 std::vector<int> pos(n + 1), que(n), id(n, -1), p(n), mate(n, -1);
 std::vector<Edge> fs(n), ret;
 auto rematch= [&](auto self, int u, int v) -> void {
  int w;
  if (w= mate[u], mate[u]= v; w == -1 || mate[w] != u) return;
  if (auto [x, y]= fs[u]; y == -1) self(self, mate[w]= x, w);
  else self(self, x, y), self(self, y, x);
 };
 int ts= 0;
 auto f= [&](auto self, int x) -> int { return id[x] != ts || p[x] == -1 ? x : (p[x]= self(self, p[x])); };
 auto check= [&](int rt) {
  fs[rt]= {-1, -1}, id[rt]= ts, p[que[0]= rt]= -1;
  for (int i= 0, s= 1; i < s; ++i) {
   int x= que[i];
   for (int y: adj[x]) {
    if (y == rt) continue;
    if (mate[y] == -1) return rematch(rematch, mate[y]= x, y), true;
    if (id[y] == ts) {
     int u= f(f, x), v= f(f, y), w= rt;
     if (u == v) continue;
     for (; u != rt || v != rt; fs[u]= {x, y}, u= f(f, fs[mate[u]].s)) {
      if (v != rt) std::swap(u, v);
      if (fs[u].s == x && fs[u].d == y) {
       w= u;
       break;
      }
     }
     for (int a= u; a != w; a= f(f, fs[mate[a]].s)) id[a]= ts, p[a]= w, que[s++]= a;
     for (int b= v; b != w; b= f(f, fs[mate[b]].s)) id[b]= ts, p[b]= w, que[s++]= b;
    } else if (id[mate[y]] != ts) fs[y]= {-1, -1}, fs[mate[y]]= {x, -1}, id[mate[y]]= ts, p[mate[y]]= y, que[s++]= mate[y];
   }
  }
  return false;
 };
 for (int rt= n; rt--;)
  if (mate[rt] == -1) ts+= check(rt);
 for (int i= 0; i < n; ++i)
  if (int j= mate[i]; i < j) ret.emplace_back(i, j);
 return ret;
}
std::vector<Edge> general_matching(const Graph &g) { return general_matching(g.adjacency_vertex(0)); }