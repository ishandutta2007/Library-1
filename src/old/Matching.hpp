#pragma once
#include <vector>
#include <array>
class Matching {
 std::vector<std::array<int, 2>> es;
 std::vector<int> mate;
public:
 Matching(int n): mate(n, -1) {}
 void add_edge(int u, int v) { es.push_back({u, v}); }
 void build() {
  const int n= mate.size();
  std::vector<int> g(es.size() * 2), pos(n + 1), que(n), id(n, -1), p(n);
  std::vector<std::array<int, 2>> fs(n);
  for (auto [u, v]: es) ++pos[u], ++pos[v];
  for (int i= 0; i < n; ++i) pos[i + 1]+= pos[i];
  for (auto [u, v]: es) g[--pos[u]]= v, g[--pos[v]]= u;
  auto rematch= [&](auto self, int u, int v) -> void {
   int w;
   if (w= mate[u], mate[u]= v; w == -1 || mate[w] != u) return;
   if (auto [x, y]= fs[u]; y == -1) self(self, mate[w]= x, w);
   else self(self, x, y), self(self, y, x);
  };
  int ts= 0;
  auto f= [&](auto self, int x) -> int { return id[x] != ts || p[x] == -1 ? x : (p[x]= self(self, p[x])); };
  auto check= [&](int rt) {
   int s= 1;
   fs[rt]= {-1, -1}, id[rt]= ts, p[que[0]= rt]= -1;
   for (int i= 0; i < s; ++i) {
    for (int x= que[i], j= pos[x], y, u, v, w, a, b; j < pos[x + 1]; ++j) {
     if (y= g[j]; y == rt) continue;
     if (mate[y] == -1) return rematch(rematch, mate[y]= x, y), true;
     if (id[y] == ts) {
      if (a= u= f(f, x), b= v= f(f, y), w= rt; u == v) continue;
      for (; u != rt || v != rt; fs[u]= {x, y}, u= f(f, fs[mate[u]][0])) {
       if (v != rt) std::swap(u, v);
       if (fs[u][0] == x && fs[u][1] == y) {
        w= u;
        break;
       }
      }
      for (; a != w; a= f(f, fs[mate[a]][0])) id[a]= ts, p[a]= w, que[s++]= a;
      for (; b != w; b= f(f, fs[mate[b]][0])) id[b]= ts, p[b]= w, que[s++]= b;
     } else if (id[mate[y]] != ts) fs[y]= {-1, -1}, fs[mate[y]]= {x, -1}, id[mate[y]]= ts, p[mate[y]]= y, que[s++]= mate[y];
    }
   }
   return false;
  };
  for (int rt= n; rt--;)
   if (mate[rt] == -1) ts+= check(rt);
 }
 int match(int u) const { return mate[u]; }
 std::vector<std::array<int, 2>> max_matching() const {
  std::vector<std::array<int, 2>> ret;
  const int n= mate.size();
  for (int i= 0; i < n; ++i)
   if (int j= mate[i]; i < j) ret.push_back({i, j});
  return ret;
 }
};