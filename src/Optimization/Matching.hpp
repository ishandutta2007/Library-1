#pragma once
#include <vector>
#include <array>
#include <queue>
class Matching {
 int n, ret;
 std::vector<std::vector<int>> adj;
 std::vector<int> mt, idx, p;
 std::vector<std::array<int, 2>> es;
 void rematch(int u, int v, int w= -1) {
  if (w= mt[u], mt[u]= v; w == -1 || mt[w] != u) return;
  if (es[u][1] == -1) {
   rematch(mt[w]= es[u][0], w);
  } else rematch(es[u][0], es[u][1]), rematch(es[u][1], es[u][0]);
 }
 int f(int x) { return (idx[x] != ret || p[x] == -1) ? x : (p[x]= f(p[x])); }
 bool check(int root) {
  std::queue<int> que;
  es[root]= {-1, -1}, idx[root]= ret, p[root]= -1, que.push(root);
  for (int x, u, v, w; !que.empty();) {
   x= que.front(), que.pop();
   for (int y: adj[x]) {
    if (y == root) continue;
    if (mt[y] == -1) {
     return rematch(mt[y]= x, y), true;
    } else if (idx[y] == ret) {
     if (u= f(x), v= f(y), w= root; u == v) continue;
     for (; u != root || v != root; es[u]= {x, y}, u= f(es[mt[u]][0])) {
      if (v != root) std::swap(u, v);
      if (es[u][0] == x && es[u][1] == y) {
       w= u;
       break;
      }
     }
     for (int t= f(x); t != w; t= f(es[mt[t]][0])) idx[t]= ret, p[t]= w, que.push(t);
     for (int t= f(y); t != w; t= f(es[mt[t]][0])) idx[t]= ret, p[t]= w, que.push(t);
    } else if (idx[mt[y]] != ret) es[y]= {-1, -1}, es[mt[y]]= {x, -1}, idx[mt[y]]= ret, p[mt[y]]= y, que.push(mt[y]);
   }
  }
  return false;
 }
public:
 Matching(int n): n(n), ret(0), adj(n), mt(n, -1), idx(n, -1), p(n), es(n) {}
 void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }
 std::pair<int, std::vector<int>> get_matching() {
  for (int i= 0; i < n; i++)
   if (mt[i] == -1) ret+= check(i);
  return std::make_pair(ret, mt);
 }
};