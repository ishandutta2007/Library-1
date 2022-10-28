#pragma once
#include <bits/stdc++.h>
/**
 * @title マトロイド交叉
 * @category 最適化問題
 *  unweighted: return 要素数最大の共通独立集合
 *  weighted: return { 要素数 = k の共通独立集合のうち重みが最大(最小)のもの }
 * @see William H. Cunningham. Improved bounds for matroid partition and
 * intersection algorithms. SIAMJournal on Computing (SICOMP), 15(4):948–957,
 * 1986.
 */
// verify用:
// https://www.beecrowd.com.br/judge/en/problems/view/2128 (グラフ+分割)

// BEGIN CUT HERE
template <typename Matroid1, typename Matroid2>
std::vector<int> matroid_intersection(int n, Matroid1 M1, Matroid2 M2) {
  std::vector<bool> b(n, false), useless(n);
  std::vector<int> I[2];
  for (int e = 0; e < n; e++) I[0].push_back(e);
  M1.build(I[1]), M2.build(I[1]);
  for (bool converged = false; !converged;) {
    useless.assign(n, false);
    std::vector L(1, std::vector<int>());
    for (int u : I[0])
      if (M1.oracle(u)) useless[u] = true, L[0].push_back(u);
    int m = 0;
    for (; L.back().size(); m += 2) {
      L.push_back({});
      for (int e : L[m]) {
        if (converged = M2.oracle(e)) break;
        for (int f : I[1])
          if (!useless[f] && M2.oracle(f, e))
            L[m + 1].push_back(f), useless[f] = true;
      }
      if (converged) break;
      L.push_back({});
      for (int e : L[m + 1])
        for (int f : I[0])
          if (!useless[f] && M1.oracle(e, f))
            L[m + 2].push_back(f), useless[f] = true;
    }
    if (!converged) break;
    std::vector<std::vector<int>> L2(m + 1);
    for (int e : L[m])
      if (M2.oracle(e)) L2[m].push_back(e);
    for (int i = m; i; i -= 2) {
      for (int e : L[i - 1])
        for (int f : L2[i])
          if (M1.oracle(e, f)) {
            L2[i - 1].push_back(e);
            break;
          }
      for (int e : L[i - 2])
        for (int f : L2[i - 1])
          if (M2.oracle(f, e)) {
            L2[i - 2].push_back(e);
            break;
          }
    }
    auto dfs = [&](auto self, int e, int i) -> bool {
      if (useless[e] = true; i == m)
        return M2.oracle(e) ? (b[e] = !b[e], true) : false;
      for (int f : L2[i + 1])
        if (!useless[f] && M2.oracle(f, e)) {
          useless[f] = true;
          for (int g : L2[i + 2])
            if (!useless[g] && M1.oracle(f, g))
              if (self(self, g, i + 2)) return b[e] = !b[e], b[f] = !b[f], true;
        }
      return false;
    };
    useless.assign(n, false);
    for (int e : L2[0])
      if (M1.oracle(e) && dfs(dfs, e, 0)) {
        converged = false, I[0].clear(), I[1].clear();
        for (int u = 0; u < n; u++) I[b[u]].push_back(u);
        M1.build(I[1]), M2.build(I[1]);
      }
  }
  return I[1];
}

// sgn: + -> max, - -> min, 0 -> unweighted
template <std::int_least8_t sgn, class Matroid1, class Matroid2, class cost_t>
std::vector<std::vector<int>> weighted_matroid_intersection(
    int n, Matroid1 M1, Matroid2 M2, std::vector<cost_t> c) {
  assert(n == (int)c.size());
  bool b[n];
  std::fill_n(b, n, false);
  std::vector<int> I[2], p;
  std::vector<std::vector<int>> ret(1);
  for (int u = 0; u < n; u++) I[0].push_back(u);
  if constexpr (sgn > 0) {
    auto cmx = *std::max_element(c.begin(), c.end());
    for (auto &x : c) x -= cmx;
  } else {
    auto cmi = *std::min_element(c.begin(), c.end());
    for (auto &x : c) x -= cmi;
  }
  for (auto &x : c) x *= sgn * (n + 1);
  for (bool converged = false; !converged;) {
    converged = true, M1.build(I[1]), M2.build(I[1]);
    std::priority_queue<std::pair<cost_t, int>> pq;
    std::vector<cost_t> dist(n, std::numeric_limits<cost_t>::lowest());
    for (int u : I[0])
      if (M1.oracle(u)) pq.emplace(dist[u] = c[u] - 1, u);
    for (p.assign(n, -1); pq.size();) {
      auto [d, u] = pq.top();
      if (pq.pop(); d != dist[u]) continue;
      if (b[u]) {
        for (int v : I[0])
          if (M1.oracle(u, v))
            if (cost_t cost = d + c[v] - 1; dist[v] < cost)
              pq.emplace(dist[v] = cost, v), p[v] = u;
      } else {
        if (M2.oracle(u)) {
          for (int v = u; v != -1; v = p[v]) b[v] = !b[v];
          I[0].clear(), I[1].clear(), converged = false;
          for (int u = 0; u < n; u++) I[b[u]].push_back(u);
          ret.emplace_back(I[1]);
          break;
        }
        for (int v : I[1])
          if (M2.oracle(v, u))
            if (cost_t cost = d - c[v] - 1; dist[v] < cost)
              pq.emplace(dist[v] = cost, v), p[v] = u;
      }
    }
  }
  return ret;
}

class GraphicMatroid {
  int n, t;
  std::vector<std::array<int, 2>> edges;
  std::vector<std::vector<int>> g;
  std::vector<int> comp, in, out;
  void dfs(int u) {
    in[u] = t++;
    for (auto v : g[u])
      if (in[v] == -1) comp[v] = comp[u], dfs(v);
    out[u] = t;
  }
  inline bool is_ancestor(int u, int v) const {
    return in[u] <= in[v] && in[v] < out[u];
  }

 public:
  GraphicMatroid(int n_) : n(n_), g(n), comp(n), in(n), out(n) {}
  void add_edge(int u, int v) { edges.emplace_back(std::array{u, v}); }
  void build(const std::vector<int> &I) {
    t = 0;
    for (int u = 0; u < n; u++) g[u].clear(), in[u] = -1;
    for (int e : I) {
      int u = edges[e][0], v = edges[e][1];
      g[u].push_back(v), g[v].push_back(u);
    }
    for (int u = 0; u < n; u++)
      if (in[u] == -1) comp[u] = u, dfs(u);
  }
  inline bool oracle(int e) const {
    return comp[edges[e][0]] != comp[edges[e][1]];
  }
  inline bool oracle(int e, int f) const {
    if (oracle(f)) return true;
    int u = edges[e][in[edges[e][0]] < in[edges[e][1]]];
    return is_ancestor(u, edges[f][0]) != is_ancestor(u, edges[f][1]);
  }
};

struct PartitionMatroid {
  std::vector<int> belong, R, cnt;
  PartitionMatroid(int m_, const std::vector<std::vector<int>> &parts,
                   const std::vector<int> &R_)
      : belong(m_, -1), R(R_) {
    assert(parts.size() == R.size());
    for (int i = parts.size(); i--;)
      for (int e : parts[i]) belong[e] = i;
  }
  void build(const std::vector<int> &I) {
    cnt = R;
    for (int e : I)
      if (belong[e] != -1) cnt[belong[e]]--;
  }
  inline bool oracle(int e) const {
    return belong[e] == -1 || cnt[belong[e]] > 0;
  }
  inline bool oracle(int e, int f) const {
    return oracle(f) || belong[e] == belong[f];
  }
};