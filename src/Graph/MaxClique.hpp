#pragma once
#include <vector>
#include <algorithm>
class MaxClique {
 const int n, m;
 using u128= __uint128_t;
 using u64= uint64_t;
 using u16= uint16_t;
 struct id_num {
  u16 id, num;
 };
 std::vector<u128> adj, buf, adj_;
 std::vector<u16> deg, clique, cur;
 void dfs(std::vector<id_num> &rem) {
  if (clique.size() < cur.size()) clique= cur;
  std::sort(rem.begin(), rem.end(), [&](id_num l, id_num r) { return deg[l.id] > deg[r.id]; });
  buf.assign((n + 1) * m, 0);
  for (auto &v: rem) {
   int b= v.id * m, bb= 0;
   for (v.num= 0;; ++v.num, bb+= m) {
    bool any= 1;
    for (int i= 0; i < m; ++i) any&= !(adj[b + i] & buf[bb + i]);
    if (any) break;
   }
   buf[bb + (v.id >> 7)]|= u128(1) << (v.id & 127);
  }
  std::sort(rem.begin(), rem.end(), [&](id_num l, id_num r) { return l.num < r.num; });
  std::vector<id_num> nrem;
  for (nrem.reserve(rem.size()); !rem.empty();) {
   auto p= rem.back();
   int a= p.id * m;
   if (p.num + cur.size() < clique.size()) break;
   nrem.clear();
   for (auto u: rem)
    if ((adj[a + (u.id >> 7)] >> (u.id & 127)) & 1) nrem.emplace_back(u);
   std::fill_n(buf.begin(), m, 0);
   for (auto u: nrem) buf[u.id >> 7]|= u128(1) << (u.id & 127);
   for (auto u: nrem) {
    int b= u.id * m, i= 0;
    for (u128 tmp; i < m; ++i) tmp= buf[i] & adj[b + i], deg[u.id]+= __builtin_popcountll(tmp >> 64) + __builtin_popcountll(u64(tmp));
   }
   cur.push_back(p.id), dfs(nrem), cur.pop_back(), rem.pop_back();
  }
 }
 void calc(bool complement) {
  adj.assign(adj_.begin(), adj_.end());
  if (complement)
   for (int u= n; u--;)
    for (int v= u; v--;) adj[u * m + (v >> 7)]^= u128(1) << (v & 127), adj[v * m + (u >> 7)]^= u128(1) << (u & 127);
  std::vector<id_num> nrem;
  for (u16 u= n; u--;) {
   nrem.push_back(id_num{u, 0});
   int b= u * m, i= 0;
   for (u128 tmp; i < m; ++i) tmp= adj[b + i], deg[u]+= __builtin_popcountll(tmp >> 64) + __builtin_popcountll(u64(tmp));
  }
  dfs(nrem);
 }
public:
 MaxClique(int n): n(n), m((n + 127) >> 7), deg(n), adj_(n * m) {}
 void add_edge(int u, int v) { adj_[u * m + (v >> 7)]|= u128(1) << (v & 127), adj_[v * m + (u >> 7)]|= u128(1) << (u & 127); }
 std::vector<u16> get_max_clique() { return calc(false), clique; }
 std::vector<u16> get_max_independent_set() { return calc(true), clique; }
 std::vector<u16> get_min_vertex_cover() {
  calc(true), std::fill_n(buf.begin(), m, 0);
  for (int u: clique) buf[u >> 7]|= u128(1) << (u & 127);
  std::vector<u16> ret;
  for (int i= 0; i < n; ++i)
   if (!((buf[i >> 7] >> (i & 127)) & 1)) ret.push_back(i);
  return ret;
 }
};