#pragma once
#include <bits/stdc++.h>
/**
 * @title HL分解
 * @category グラフ
 * 可換性を要求
 */

// BEGIN CUT HERE

struct HeavyLightDecomposition {
  std::vector<std::vector<int>> adj;
  std::vector<int> sz, in, out, head, par;

  HeavyLightDecomposition(int n) : adj(n) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  void build(int rt = 0) {
    sz.assign(adj.size(), 1);
    in.assign(adj.size(), 0);
    out.assign(adj.size(), 0);
    head.assign(adj.size(), 0);
    par.assign(adj.size(), 0);
    dfs_sz(rt, -1);
    int t = 0;
    dfs_hld(rt, -1, t);
  }
  int lca(int u, int v) const {
    for (;; v = par[head[v]]) {
      if (in[u] > in[v]) std::swap(u, v);
      if (head[u] == head[v]) return u;
    }
  }
  template <typename T, typename Q, typename F>
  T query_path(int u, int v, const Q &q, const F &f, const T ti,
               bool edge = false) {
    T vl = ti, vr = ti;
    for (;; v = par[head[v]]) {
      if (in[u] > in[v]) std::swap(u, v), std::swap(vl, vr);
      if (head[u] == head[v]) break;
      vl = f(q(in[head[v]], in[v] + 1), vl);
    }
    return f(f(q(in[u] + edge, in[v] + 1), vl), vr);
  }
  template <typename T, typename Q>
  T query_subtree(int v, const Q &q, bool edge = false) {
    return q(in[v] + edge, out[v]);
  }

  template <typename Q>
  void update_path(int u, int v, const Q &q, bool edge = false) {
    for (;; v = par[head[v]]) {
      if (in[u] > in[v]) std::swap(u, v);
      if (head[u] == head[v]) break;
      q(in[head[v]], in[v] + 1);
    }
    q(in[u] + edge, in[v] + 1);
  }
  template <typename Q>
  void update_subtree(int v, const Q &q, bool edge = false) {
    q(in[v] + edge, out[v]);
  }

 private:
  void dfs_sz(int v, int p) {
    if (adj[v].size() && adj[v][0] == p) std::swap(adj[v][0], adj[v].back());
    for (auto &u : adj[v])
      if (u != p) {
        dfs_sz(u, v);
        sz[v] += sz[u];
        if (sz[adj[v][0]] < sz[u]) std::swap(adj[v][0], u);
      }
  }
  void dfs_hld(int v, int p, int &times) {
    par[v] = p;
    in[v] = times++;
    for (auto &u : adj[v])
      if (u != p) {
        head[u] = (adj[v][0] == u ? head[v] : u);
        dfs_hld(u, v, times);
      }
    out[v] = times;
  }
};
