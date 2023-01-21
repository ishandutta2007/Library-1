#pragma once
#include <vector>
#include <algorithm>
class HeavyLightDecomposition {
 std::vector<std::vector<int>> adj;
 std::vector<int> sz, in, out, head, par;
 void dfs_sz(int v, int p) {
  if (adj[v].size() && adj[v][0] == p) std::swap(adj[v][0], adj[v].back());
  for (auto &u: adj[v])
   if (u != p)
    if (dfs_sz(u, v), sz[v]+= sz[u]; sz[adj[v][0]] < sz[u]) std::swap(adj[v][0], u);
 }
 void dfs_hld(int v, int p, int &ts) {
  par[v]= p, in[v]= ts++;
  for (auto &u: adj[v])
   if (u != p) head[u]= (adj[v][0] == u ? head[v] : u), dfs_hld(u, v, ts);
  out[v]= ts;
 }
public:
 HeavyLightDecomposition(int n): adj(n), sz(n, 1), in(n, 0), out(n, 0), head(n, 0), par(n, 0) {}
 void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }
 void build(int rt= 0) {
  int timestamp= 0;
  dfs_sz(rt, -1), dfs_hld(rt, -1, timestamp);
 }
 int lca(int u, int v) const {
  for (;; v= par[head[v]]) {
   if (in[u] > in[v]) std::swap(u, v);
   if (head[u] == head[v]) return u;
  }
 }
 template <typename T, typename Q, typename F> T fold_path(int u, int v, const Q &q, const F &f, const T ti, bool edge= false) {
  T vl= ti, vr= ti;
  for (;; vl= f(q(in[head[v]], in[v] + 1), vl), v= par[head[v]]) {
   if (in[u] > in[v]) std::swap(u, v), std::swap(vl, vr);
   if (head[u] == head[v]) break;
  }
  return f(f(q(in[u] + edge, in[v] + 1), vl), vr);
 }
 template <typename T, typename Q> T fold_subtree(int v, const Q &q, bool edge= false) { return q(in[v] + edge, out[v]); }
 template <typename Q> void apply_path(int u, int v, const Q &q, bool edge= false) {
  for (;; q(in[head[v]], in[v] + 1), v= par[head[v]]) {
   if (in[u] > in[v]) std::swap(u, v);
   if (head[u] == head[v]) break;
  }
  q(in[u] + edge, in[v] + 1);
 }
 template <typename Q> void apply_subtree(int v, const Q &q, bool edge= false) { q(in[v] + edge, out[v]); }
 int operator[](int i) const { return in[i]; }
};