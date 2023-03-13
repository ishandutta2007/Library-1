#pragma once
#include <vector>
#include <algorithm>
#include <limits>
#include <array>
#include <queue>
#include <cassert>
template <typename Matroid1, typename Matroid2> std::vector<int> matroid_intersection(int n, Matroid1 M1, Matroid2 M2) {
 std::vector<int> b(n, false), pre(n), I[2];
 for (int e= 0; e < n; e++) I[0].push_back(e);
 M1.build(I[1]), M2.build(I[1]);
 for (bool converged= false; !converged;) {
  pre.assign(n, false);
  std::vector L(1, std::vector<int>());
  for (int u: I[0])
   if (M1.oracle(u)) pre[u]= true, L[0].push_back(u);
  int m= 0;
  for (; L.back().size(); m+= 2) {
   L.push_back({});
   for (int e: L[m]) {
    if (converged= M2.oracle(e)) break;
    for (int f: I[1])
     if (!pre[f] && M2.oracle(f, e)) L[m + 1].push_back(f), pre[f]= true;
   }
   if (converged) break;
   L.push_back({});
   for (int e: L[m + 1])
    for (int f: I[0])
     if (!pre[f] && M1.oracle(e, f)) L[m + 2].push_back(f), pre[f]= true;
  }
  if (!converged) break;
  std::vector<std::vector<int>> L2(m + 1);
  for (int e: L[m])
   if (M2.oracle(e)) L2[m].push_back(e);
  for (int i= m; i; i-= 2) {
   for (int e: L[i - 1])
    for (int f: L2[i])
     if (M1.oracle(e, f)) {
      L2[i - 1].push_back(e);
      break;
     }
   for (int e: L[i - 2])
    for (int f: L2[i - 1])
     if (M2.oracle(f, e)) {
      L2[i - 2].push_back(e);
      break;
     }
  }
  pre.assign(n, -1);
  for (int e: L2[0])
   if (M1.oracle(e)) {
    bool isok= false;
    if (m) {
     std::vector<size_t> ei(m);
     for (int i= 0; e != -1;) {
      if (ei[i] == L2[i + 1].size()) e= pre[e], ei[i--]= 0;
      else if (int f= L2[i + 1][ei[i]++]; pre[f] == -1 && (i & 1 ? M1.oracle(e, f) : M2.oracle(f, e)))
       if (pre[f]= e, e= f; ++i == m) {
        if (M2.oracle(e))
         for (isok= true; e != -1; e= pre[e]) b[e]= !b[e];
        else e= pre[e], --i;
       }
     }
    } else if (M2.oracle(e)) isok= true, b[e]= 1;
    if (isok) {
     converged= false, I[0].clear(), I[1].clear();
     for (int u= 0; u < n; u++) I[b[u]].push_back(u);
     M1.build(I[1]), M2.build(I[1]);
    }
   }
 }
 return I[1];
}
// sgn: + -> max, - -> min, 0 -> unweighted
template <std::int_least8_t sgn, class Matroid1, class Matroid2, class cost_t> std::vector<std::vector<int>> weighted_matroid_intersection(int n, Matroid1 M1, Matroid2 M2, std::vector<cost_t> c) {
 assert(n == (int)c.size());
 bool b[n];
 std::fill_n(b, n, false);
 std::vector<int> I[2], p;
 std::vector<std::vector<int>> ret(1);
 for (int u= 0; u < n; u++) I[0].push_back(u);
 if constexpr (sgn > 0) {
  auto cmx= *std::max_element(c.begin(), c.end());
  for (auto &x: c) x-= cmx;
 } else {
  auto cmi= *std::min_element(c.begin(), c.end());
  for (auto &x: c) x-= cmi;
 }
 for (auto &x: c) x*= sgn * (n + 1);
 for (bool converged= false; !converged;) {
  converged= true, M1.build(I[1]), M2.build(I[1]);
  std::priority_queue<std::pair<cost_t, int>> pq;
  std::vector<cost_t> dist(n, std::numeric_limits<cost_t>::lowest());
  for (int u: I[0])
   if (M1.oracle(u)) pq.emplace(dist[u]= c[u] - 1, u);
  for (p.assign(n, -1); pq.size();) {
   auto [d, u]= pq.top();
   if (pq.pop(); d != dist[u]) continue;
   if (b[u]) {
    for (int v: I[0])
     if (M1.oracle(u, v))
      if (cost_t cost= d + c[v] - 1; dist[v] < cost) pq.emplace(dist[v]= cost, v), p[v]= u;
   } else {
    if (M2.oracle(u)) {
     for (int v= u; v != -1; v= p[v]) b[v]= !b[v];
     I[0].clear(), I[1].clear(), converged= false;
     for (int u= 0; u < n; u++) I[b[u]].push_back(u);
     ret.emplace_back(I[1]);
     break;
    }
    for (int v: I[1])
     if (M2.oracle(v, u))
      if (cost_t cost= d - c[v] - 1; dist[v] < cost) pq.emplace(dist[v]= cost, v), p[v]= u;
   }
  }
 }
 return ret;
}
class GraphicMatroid {
 int n;
 std::vector<std::array<int, 2>> es;
 std::vector<int> g, pos, comp, in, out;
 inline bool is_ancestor(int u, int v) const { return in[u] <= in[v] && in[v] < out[u]; }
public:
 GraphicMatroid(int n_): n(n_), comp(n), in(n), out(n) {}
 void add_edge(int u, int v) { es.push_back({u, v}); }
 void build(const std::vector<int> &I) {
  in.assign(n, -1), g.resize(I.size() * 2), pos.assign(n + 1, 0);
  for (int e: I) {
   auto [u, v]= es[e];
   ++pos[u], ++pos[v];
  }
  for (int i= 0; i < n; ++i) pos[i + 1]+= pos[i];
  for (int e: I) {
   auto [u, v]= es[e];
   g[--pos[u]]= v, g[--pos[v]]= u;
  }
  std::vector<int> ei(pos.begin(), pos.begin() + n), pre(n, -1);
  for (int u= 0, t= 0, p; u < n; u++)
   if (in[u] == -1)
    for (in [comp[u]= p= u]= t++; p >= 0;) {
     if (ei[p] == pos[p + 1]) out[p]= t, p= pre[p];
     else if (int v= g[ei[p]++]; in[v] == -1) comp[v]= comp[u], pre[v]= p, in[p= v]= t++;
    }
 }
 inline bool oracle(int e) const { return comp[es[e][0]] != comp[es[e][1]]; }
 inline bool oracle(int e, int f) const {
  if (oracle(f)) return true;
  return e= es[e][in[es[e][0]] < in[es[e][1]]], is_ancestor(e, es[f][0]) != is_ancestor(e, es[f][1]);
 }
};
struct PartitionMatroid {
 std::vector<int> belong, R, cnt;
 PartitionMatroid(int m_, const std::vector<std::vector<int>> &parts, const std::vector<int> &R_): belong(m_, -1), R(R_) {
  assert(parts.size() == R.size());
  for (int i= parts.size(); i--;)
   for (int e: parts[i]) belong[e]= i;
 }
 void build(const std::vector<int> &I) {
  cnt= R;
  for (int e: I)
   if (belong[e] != -1) cnt[belong[e]]--;
 }
 inline bool oracle(int e) const { return belong[e] == -1 || cnt[belong[e]] > 0; }
 inline bool oracle(int e, int f) const { return oracle(f) || belong[e] == belong[f]; }
};