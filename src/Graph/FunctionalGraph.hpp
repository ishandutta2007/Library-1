#pragma once
#include "src/Graph/Tree.hpp"
class FunctionalGraph {
 std::vector<int> to, rt;
 Tree<> tree;
public:
 FunctionalGraph(int n): to(n, -1), rt(n, -1), tree(n + 1) {}
 void add_edge(int src, int dst) { assert(to[src] == -1), to[src]= dst; }
 void build() {
  const int n= to.size();
  for (int u, w, v= n; v--;)
   if (rt[v] == -1) {
    for (rt[v]= -2, w= to[v];; rt[w]= -2, w= to[w])
     if (assert(w != -1); rt[w] != -1) {
      if (rt[w] != -2) w= rt[w];
      break;
     }
    for (u= v; rt[u] == -2; u= to[u]) rt[u]= w;
   }
  for (int v= n; v--;)
   if (rt[v] == v) tree.add_edge(v, n);
   else tree.add_edge(v, to[v]);
  tree.build(n);
 }
 template <class Int> std::enable_if_t<std::is_convertible_v<int, Int>, int> jump(int v, Int k) const {
  int n= to.size(), d= tree.depth(v) - 1;
  if (k <= d) return tree.jump(v, n, (int)k);
  int b= to[v= rt[v]], l= (k-= d) % tree.depth(b);
  if (l == 0) return v;
  return tree.jump(b, n, l - 1);
 }
 // ((a_0,...,a_{i-1}) x 1, (a_i,...,a_{j-1}) x loop_num, (a_j,...,a_m) x 1)
 template <class Int> std::enable_if_t<std::is_convertible_v<int, Int>, std::array<std::pair<std::vector<int>, Int>, 3>> path(int v, Int k) const {
  std::array<std::pair<std::vector<int>, Int>, 3> ret;
  int n= to.size(), d= tree.depth(v) - 1;
  if (ret[0].second= 1; k <= d) {
   for (int e= k; e--; v= to[v]) ret[0].first.push_back(v);
   return ret;
  }
  for (int e= d; e--; v= to[v]) ret[0].first.push_back(v);
  int b= to[v= rt[v]], c= tree.depth(b), l= (k-= d) % c;
  ret[1].second= k / c, ret[2].second= 1;
  for (int e= c; e--; v= to[v]) ret[1].first.push_back(v);
  for (int e= l; e--; v= to[v]) ret[2].first.push_back(v);
  return ret;
 }
};