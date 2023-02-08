#pragma once
#include <type_traits>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <array>
#include <numeric>
#include <cassert>
template <class Cost= void> class Tree {
 template <class D, class T> struct Edge_B {
  int to;
  T cost;
 };
 template <class D> struct Edge_B<D, void> { int to; };
 using Edge= Edge_B<void, Cost>;
 std::vector<std::vector<Edge>> adj;
 std::vector<int> P, PP, D, I, L, R;
public:
 Tree(int n): adj(n) {}
 template <class T= Cost, std::enable_if_t<std::is_same_v<T, void>, std::nullptr_t> = nullptr> void add_edge(int u, int v) { adj[u].emplace_back(Edge{v}), adj[v].emplace_back(Edge{u}); }
 template <class T, std::enable_if_t<std::is_convertible_v<T, Cost>, std::nullptr_t> = nullptr> void add_edge(int u, int v, T c) { adj[u].emplace_back(Edge{v, c}), adj[v].emplace_back(Edge{u, c}); }
 template <class T, class U, std::enable_if_t<std::conjunction_v<std::is_convertible<T, Cost>, std::is_convertible<U, Cost>>, std::nullptr_t> = nullptr> void add_edge(int u, int v, T c, U d) /* c:u->v, d:v->u */ { adj[u].emplace_back(Edge{v, c}), adj[v].emplace_back(Edge{u, d}); }
 const std::vector<Edge> &operator[](int v) const { return adj[v]; }
 void build(int root= 0) {
  const int n= adj.size();
  P.assign(n, -2), I.reserve(n), PP.resize(n), std::iota(PP.begin(), PP.end(), 0), D.assign(n, 0), L.assign(n, 0), R.assign(n, 0);
  auto f= [&, i= 0, v= 0](int r) mutable {
   for (P[r]= -1, I.push_back(r); i < (int)I.size(); ++i)
    for (const Edge &e: adj[v= I[i]])
     if (P[v] != e.to) I.push_back(e.to), P[e.to]= v;
  };
  f(root);
  for (int r= 0; r < n; ++r)
   if (P[r] == -2) f(r);
  std::vector<int> Z(n, 1), nx(n, -1);
  for (int i= n, v; i--;) {
   if (P[v= I[i]] == -1) continue;
   if (Z[P[v]]+= Z[v]; nx[P[v]] == -1) nx[P[v]]= v;
   if (Z[nx[P[v]]] < Z[v]) nx[P[v]]= v;
  }
  for (int v: I)
   if (nx[v] != -1) PP[nx[v]]= v;
  for (int v: I)
   if (P[v] != -1) PP[v]= PP[PP[v]], D[v]= D[P[v]] + 1;
  for (int i= 0; i < n; ++i) L[I[i]]= i;
  for (int v: I) {
   int ir= R[v]= L[v] + Z[v];
   for (const Edge &e: adj[v])
    if (int u= e.to; u != P[v] && u != nx[v]) L[u]= ir-= Z[u];
   if (nx[v] != -1) L[nx[v]]= L[v] + 1;
  }
  for (int i= 0; i < n; ++i) I[L[i]]= i;
 }
 int size() const { return adj.size(); }
 bool builded() const { return L.size() == adj.size(); }
 int depth(int v) const { return assert(builded()), D[v]; }
 int to_seq(int v) const { return assert(builded()), L[v]; }
 int to_node(int i) const { return assert(builded()), I[i]; }
 int parent(int v) const { return assert(builded()), P[v]; }
 int root(int v) const {
  for (assert(builded()), v= PP[v];; v= PP[P[v]])
   if (P[v] == -1) return v;
 }
 bool connected(int u, int v) const { return root(u) == root(v); }
 int lca(int u, int v) const {
  for (assert(builded());; v= P[PP[v]]) {
   if (L[u] > L[v]) std::swap(u, v);
   if (PP[u] == PP[v]) return u;
  }
 }
 int la(int v, int k) const {
  assert(builded()), assert(k <= D[v]);
  for (int u;; k-= L[v] - L[u] + 1, v= P[u])
   if (L[v] - k >= L[u= PP[v]]) return I[L[v] - k];
 }
 int jump(int u, int v, int k) const {
  if (assert(builded()); u == v) return -1;
  if (k == 1) return in_subtree(v, u) ? la(v, D[v] - D[u] - 1) : P[u];
  int w= lca(u, v), d_uw= D[u] - D[w], d_vw= D[v] - D[w];
  return k > d_uw + d_vw ? -1 : k <= d_uw ? la(u, k) : la(v, d_uw + d_vw - k);
 }
 int dist(int u, int v) const { return assert(builded()), depth(u) + depth(v) - depth(lca(u, v)) * 2; }
 bool in_subtree(int u, int v) /* u is in v */ const { return assert(builded()), L[v] <= L[u] && L[u] < R[v]; }
 int subtree_size(int v) const { return assert(builded()), R[v] - L[v]; }
 std::array<int, 2> subtree(int v) /* half-open interval */ const { return assert(builded()), std::array{L[v], R[v]}; }
 template <bool edge= 0> std::vector<std::array<int, 2>> path(int u, int v) /* sequence of closed intervals */ const {
  assert(builded());
  std::vector<std::array<int, 2>> up, down;
  while (PP[u] != PP[v]) {
   if (L[u] < L[v]) down.emplace_back(std::array{L[PP[v]], L[v]}), v= P[PP[v]];
   else up.emplace_back(std::array{L[u], L[PP[u]]}), u= P[PP[u]];
  }
  if (L[u] < L[v]) down.emplace_back(std::array{L[u] + edge, L[v]});
  else if (L[v] + edge <= L[u]) up.emplace_back(std::array{L[u], L[v] + edge});
  return up.insert(up.end(), down.rbegin(), down.rend()), up;
 }
};