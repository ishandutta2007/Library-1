#pragma once
#include "src/Graph/HeavyLightDecomposition.hpp"
class FunctionalGraph {
 std::vector<int> t, rt;
 HeavyLightDecomposition hld;
public:
 FunctionalGraph(const std::vector<int> &to): t(to) {
  const int n= t.size();
  rt.assign(n, -1);
  for (int u, w, v= n; v--;)
   if (rt[v] == -1) {
    for (rt[v]= -2, w= t[v];; rt[w]= -2, w= t[w])
     if (assert(0 <= w && w < n); rt[w] != -1) {
      if (rt[w] != -2) w= rt[w];
      break;
     }
    for (u= v; rt[u] == -2; u= t[u]) rt[u]= w;
   }
  Graph g(n);
  for (int v= n; v--;) g[v]= {(rt[v] == v ? n : t[v]), v};
  g.build(n + 1, 1), hld= HeavyLightDecomposition(g, n);
 }
 template <class Int> std::enable_if_t<std::is_convertible_v<int, Int>, int> jump(int v, Int k) const {
  int n= t.size(), d= hld.depth(v) - 1;
  if (k <= d) return hld.jump(v, n, (int)k);
  int b= t[v= rt[v]], l= (k-= d) % hld.depth(b);
  if (l == 0) return v;
  return hld.jump(b, n, l - 1);
 }
 // ((a_0,...,a_{i-1}) x 1, (a_i,...,a_{j-1}) x loop_num, (a_j,...,a_m) x 1)
 template <class Int> std::enable_if_t<std::is_convertible_v<int, Int>, std::array<std::pair<std::vector<int>, Int>, 3>> path(int v, Int k) const {
  std::array<std::pair<std::vector<int>, Int>, 3> ret;
  int n= t.size(), d= hld.depth(v) - 1;
  if (ret[0].second= 1; k <= d) {
   for (int e= k; e--; v= t[v]) ret[0].first.push_back(v);
   return ret;
  }
  for (int e= d; e--; v= t[v]) ret[0].first.push_back(v);
  int b= t[v= rt[v]], c= hld.depth(b), l= (k-= d) % c;
  ret[1].second= k / c, ret[2].second= 1;
  for (int e= c; e--; v= t[v]) ret[1].first.push_back(v);
  for (int e= l; e--; v= t[v]) ret[2].first.push_back(v);
  return ret;
 }
};