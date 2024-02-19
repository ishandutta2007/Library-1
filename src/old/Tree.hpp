#pragma once
#include <type_traits>
#include <cstddef>
#include <algorithm>
#include <array>
#include <tuple>
#include <numeric>
#include <cassert>
#include "src/Internal/ListRange.hpp"
template <class Cost= void, bool weight= false> class Tree {
 template <class D, class T> struct Edge_B {
  int to;
  T cost;
  operator int() const { return to; }
 };
 template <class D> struct Edge_B<D, void> {
  int to;
  operator int() const { return to; }
 };
 using Edge= Edge_B<void, Cost>;
 using C= std::conditional_t<std::is_void_v<Cost>, std::nullptr_t, Cost>;
 std::vector<std::conditional_t<std::is_void_v<Cost>, std::pair<int, int>, std::tuple<int, int, Cost>>> es;
 std::vector<Edge> g;
 std::vector<int> P, PP, D, I, L, R, pos;
 std::vector<C> DW, W;
public:
 Tree(int n): P(n, -2) {}
 template <class T= Cost> std::enable_if_t<std::is_void_v<T>, void> add_edge(int u, int v) { es.emplace_back(u, v), es.emplace_back(v, u); }
 template <class T> std::enable_if_t<std::is_convertible_v<T, Cost>, void> add_edge(int u, int v, T c) { es.emplace_back(u, v, c), es.emplace_back(v, u, c); }
 template <class T, class U, std::enable_if_t<std::conjunction_v<std::is_convertible<T, Cost>, std::is_convertible<U, Cost>>, std::nullptr_t> = nullptr> void add_edge(int u, int v, T c, U d) /* c:u->v, d:v->u */ { es.emplace_back(u, v, c), es.emplace_back(v, u, d); }
 void build(int root= 0) {
  size_t n= P.size();
  I.resize(n), PP.resize(n), std::iota(PP.begin(), PP.end(), 0), D.assign(n, 0), L.assign(n, 0), R.assign(n, 0), pos.resize(n + 1), g.resize(es.size());
  for (const auto &e: es) ++pos[std::get<0>(e)];
  std::partial_sum(pos.begin(), pos.end(), pos.begin());
  if constexpr (std::is_void_v<Cost>)
   for (const auto &[f, t]: es) g[--pos[f]]= {t};
  else
   for (const auto &[f, t, c]: es) g[--pos[f]]= {t, c};
  auto f= [&, i= 0, v= 0, t= 0](int r) mutable {
   for (P[r]= -1, I[t++]= r; i < t; ++i)
    for (int u: operator[](v= I[i]))
     if (P[v] != u) P[I[t++]= u]= v;
  };
  f(root);
  for (size_t r= 0; r < n; ++r)
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
  for (int i= n; i--;) L[I[i]]= i;
  for (int v: I) {
   int ir= R[v]= L[v] + Z[v];
   for (int u: operator[](v))
    if (u != P[v] && u != nx[v]) L[u]= ir-= Z[u];
   if (nx[v] != -1) L[nx[v]]= L[v] + 1;
  }
  if constexpr (weight) {
   DW.resize(n), W.resize(n);
   for (int v: I)
    for (auto &[u, c]: operator[](v)) {
     if (u != P[v]) DW[u]= DW[v] + c;
     else W[v]= c;
    }
  }
  for (int i= n; i--;) I[L[i]]= i;
 }
 size_t size() const { return P.size(); }
 ConstListRange<Edge> operator[](int v) const { return {g.cbegin() + pos[v], g.cbegin() + pos[v + 1]}; }
 int depth(int v) const { return D[v]; }
 C depth_w(int v) const {
  static_assert(weight, "\'depth_w\' is not available");
  return DW[v];
 }
 int to_seq(int v) const { return L[v]; }
 int to_node(int i) const { return I[i]; }
 int parent(int v) const { return P[v]; }
 int head(int v) const { return PP[v]; }
 int root(int v) const {
  for (v= PP[v];; v= PP[P[v]])
   if (P[v] == -1) return v;
 }
 bool connected(int u, int v) const { return root(u) == root(v); }
 int lca(int u, int v) const {
  for (;; v= P[PP[v]]) {
   if (L[u] > L[v]) std::swap(u, v);
   if (PP[u] == PP[v]) return u;
  }
 }
 int la(int v, int k) const {
  assert(k <= D[v]);
  for (int u;; k-= L[v] - L[u] + 1, v= P[u])
   if (L[v] - k >= L[u= PP[v]]) return I[L[v] - k];
 }
 int la_w(int v, C w) const {
  static_assert(weight, "\'la_w\' is not available");
  for (C c;; w-= c) {
   int u= PP[v];
   if (c= DW[v] - DW[u] + W[u]; w < c) {
    int ok= L[v], ng= L[u] - 1;
    for (int m; ok - ng > 1;) m= (ok + ng) / 2, (DW[v] - DW[I[m]] <= w ? ok : ng)= m;
    return I[ok];
   }
   if (v= P[u]; v == -1) return u;
  }
 }
 int jump(int u, int v, int k) const {
  if (!k) return u;
  if (u == v) return -1;
  if (k == 1) return in_subtree(v, u) ? la(v, D[v] - D[u] - 1) : P[u];
  int w= lca(u, v), d_uw= D[u] - D[w], d_vw= D[v] - D[w];
  return k > d_uw + d_vw ? -1 : k <= d_uw ? la(u, k) : la(v, d_uw + d_vw - k);
 }
 int jump_w(int u, int v, C w) const {
  static_assert(weight, "\'jump_w\' is not available");
  if (u == v) return u;
  int z= lca(u, v);
  C d_uz= DW[u] - DW[z], d_vz= DW[v] - DW[z];
  return w >= d_uz + d_vz ? v : w <= d_uz ? la_w(u, w) : la_w(v, d_uz + d_vz - w);
 }
 int dist(int u, int v) const { return D[u] + D[v] - D[lca(u, v)] * 2; }
 C dist_w(int u, int v) const {
  static_assert(weight, "\'dist_w\' is not available");
  return DW[u] + DW[v] - DW[lca(u, v)] * 2;
 }
 // u is in v
 bool in_subtree(int u, int v) const { return L[v] <= L[u] && L[u] < R[v]; }
 int subtree_size(int v) const { return R[v] - L[v]; }
 // half-open interval
 std::array<int, 2> subtree(int v) const { return std::array{L[v], R[v]}; }
 // sequence of closed intervals
 template <bool edge= 0> std::vector<std::array<int, 2>> path(int u, int v) const {
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