#pragma once
#include <array>
#include <cassert>
#include "src/Graph/Graph.hpp"
class HeavyLightDecomposition {
 std::vector<int> P, PP, D, I, L, R;
public:
 HeavyLightDecomposition() {}
 HeavyLightDecomposition(const Graph &tree, int root= 0) {
  const int n= tree.vertex_size();
  P.assign(n, -2), PP.resize(n), D.resize(n), I.resize(n), L.resize(n), R.resize(n);
  auto f= [&, i= 0, v= 0, t= 0](int r) mutable {
   for (P[r]= -1, I[t++]= r; i < t; ++i)
    for (int e: tree(v= I[i]))
     if (int u= tree[e] - v; P[v] != u) P[I[t++]= u]= v;
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
  for (int v= n; v--;) PP[v]= v;
  for (int v: I)
   if (nx[v] != -1) PP[nx[v]]= v;
  for (int v: I)
   if (P[v] != -1) PP[v]= PP[PP[v]], D[v]= D[P[v]] + 1;
  for (int i= n; i--;) L[I[i]]= i;
  for (int v: I) {
   int ir= R[v]= L[v] + Z[v];
   for (int e: tree(v))
    if (int u= tree[e] - v; u != P[v] && u != nx[v]) L[u]= (ir-= Z[u]);
   if (nx[v] != -1) L[nx[v]]= L[v] + 1;
  }
  for (int i= n; i--;) I[L[i]]= i;
 }
 int to_seq(int v) const { return L[v]; }
 int to_vertex(int i) const { return I[i]; }
 size_t size() const { return P.size(); }
 int parent(int v) const { return P[v]; }
 int head(int v) const { return PP[v]; }
 int root(int v) const {
  for (v= PP[v];; v= PP[P[v]])
   if (P[v] == -1) return v;
 }
 bool connected(int u, int v) const { return root(u) == root(v); }
 // u is in v
 bool in_subtree(int u, int v) const { return L[v] <= L[u] && L[u] < R[v]; }
 int subtree_size(int v) const { return R[v] - L[v]; }
 int lca(int u, int v) const {
  for (;; v= P[PP[v]]) {
   if (L[u] > L[v]) std::swap(u, v);
   if (PP[u] == PP[v]) return u;
  }
 }
 int la(int v, int k) const {
  assert(0 <= k && k <= D[v]);
  for (int u;; k-= L[v] - L[u] + 1, v= P[u])
   if (L[v] - k >= L[u= PP[v]]) return I[L[v] - k];
 }
 int jump(int u, int v, int k) const {
  if (!k) return u;
  if (u == v) return -1;
  if (k == 1) return in_subtree(v, u) ? la(v, D[v] - D[u] - 1) : P[u];
  int w= lca(u, v), d_uw= D[u] - D[w], d_vw= D[v] - D[w];
  return k > d_uw + d_vw ? -1 : k <= d_uw ? la(u, k) : la(v, d_uw + d_vw - k);
 }
 int depth(int v) const { return D[v]; }
 int dist(int u, int v) const { return D[u] + D[v] - D[lca(u, v)] * 2; }
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