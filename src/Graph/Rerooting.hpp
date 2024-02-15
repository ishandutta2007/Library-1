#pragma once
#include "src/Graph/HeavyLightDecomposition.hpp"
// put_edge(int v, int e, T t) -> U
// op(U l, U r) -> U
// ui(:U) is the identity element of op
// put_vertex(int v, U sum) -> T
template <class T> class Rerooting {
 const HeavyLightDecomposition &hld;
 std::vector<T> dp, dp1, dp2;
public:
 template <class U, class F1, class F2, class F3> Rerooting(const Graph &g, const CSRArray<int> &adje, const HeavyLightDecomposition &hld, const F1 &put_edge, const F2 &op, const U &ui, const F3 &put_vertex): hld(hld) {
  static_assert(std::is_invocable_r_v<U, F1, int, int, T>, "put_edge(int,int,T) is not invocable");
  static_assert(std::is_invocable_r_v<U, F2, U, U>, "op(U,U) is not invocable");
  static_assert(std::is_invocable_r_v<T, F3, int, U>, "put_vertex(int,U) is not invocable");
  const int n= g.vertex_size();
  dp.resize(n), dp1.resize(n), dp2.resize(n);
  for (int i= n, v; i--;) {
   U sum= ui;
   for (int e: adje[v= hld.to_vertex(i)])
    if (int u= g[e].to(v); u != hld.parent(v)) sum= op(sum, put_edge(v, e, dp1[u]));
   dp1[v]= put_vertex(v, sum);
  }
  for (int i= 0, v; i < n; ++i) {
   auto gv= adje[v= hld.to_vertex(i)];
   int dg= gv.size();
   std::vector<U> f(dg + 1), b(dg + 1);
   for (int j= 0, e, u; j < dg; ++j) u= g[e= gv[j]].to(v), f[j + 1]= put_edge(v, e, u == hld.parent(v) ? dp2[v] : dp1[u]);
   f[0]= b[dg]= ui;
   for (int j= dg; j--;) b[j]= op(f[j + 1], b[j + 1]);
   for (int j= 0; j < dg; ++j) f[j + 1]= op(f[j], f[j + 1]);
   for (int j= 0; j < dg; ++j)
    if (int u= g[gv[j]].to(v); u != hld.parent(v)) dp2[u]= put_vertex(v, op(f[j], b[j + 1]));
   dp[v]= put_vertex(v, f[dg]);
  }
 }
 template <class U, class F1, class F2, class F3> Rerooting(const Graph &g, const CSRArray<int> &adje, const F1 &put_edge, const F2 &op, const U &ui, const F3 &put_vertex): Rerooting(g, adje, HeavyLightDecomposition(g), put_edge, op, ui, put_vertex) {}
 template <class U, class F1, class F2, class F3> Rerooting(const Graph &g, const HeavyLightDecomposition &hld, const F1 &put_edge, const F2 &op, const U &ui, const F3 &put_vertex): Rerooting(g, g.adjacency_edge(0), hld, put_edge, op, ui, put_vertex) {}
 template <class U, class F1, class F2, class F3> Rerooting(const Graph &g, const F1 &put_edge, const F2 &op, const U &ui, const F3 &put_vertex): Rerooting(g, g.adjacency_edge(0), HeavyLightDecomposition(g), put_edge, op, ui, put_vertex) {}
 const T &operator[](int v) const { return dp[v]; }
 auto begin() const { return dp.cbegin(); }
 auto end() const { return dp.cend(); }
 const T &operator()(int root, int v) const { return root == v ? dp[v] : hld.in_subtree(root, v) ? dp2[hld.jump(v, root, 1)] : dp1[v]; }
};