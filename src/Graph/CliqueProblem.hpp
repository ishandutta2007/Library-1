#pragma once
#include <vector>
#include <algorithm>
class CliqueProblem {
 using u128= __uint128_t;
 using u64= uint64_t;
 using u16= uint16_t;
 const u16 n, m;
 struct id_num {
  u16 id, num;
 };
 std::vector<u128> adj_;
 std::vector<u16> calc(bool complement) const {
  std::vector<u128> buf, adj(adj_);
  std::vector<u16> deg(n), clique, cur;
  if (complement)
   for (int u= n; u--;)
    for (int v= u; v--;) adj[u * m + (v >> 7)]^= u128(1) << (v & 127), adj[v * m + (u >> 7)]^= u128(1) << (u & 127);
  auto dfs= [&](auto dfs, std::vector<id_num> &rem) -> void {
   if (clique.size() < cur.size()) clique= cur;
   std::sort(rem.begin(), rem.end(), [&](id_num l, id_num r) { return deg[l.id] > deg[r.id]; }), buf.assign((n + 1) * m, 0);
   for (auto &v: rem) {
    int b= v.id * m, bb= 0;
    for (v.num= 0;; ++v.num, bb+= m) {
     bool any= 1;
     for (u16 i= 0; i < m; ++i) any&= !(adj[b + i] & buf[bb + i]);
     if (any) break;
    }
    buf[bb + (v.id >> 7)]|= u128(1) << (v.id & 127);
   }
   std::sort(rem.begin(), rem.end(), [&](id_num l, id_num r) { return l.num < r.num; });
   std::vector<id_num> nrem;
   for (nrem.reserve(rem.size()); !rem.empty();) {
    auto p= rem.back();
    if (p.num + cur.size() < clique.size()) break;
    nrem.clear();
    auto a= adj.cbegin() + p.id * m;
    for (auto u: rem)
     if ((a[u.id >> 7] >> (u.id & 127)) & 1) nrem.emplace_back(u);
    std::fill_n(buf.begin(), m, 0);
    for (auto u: nrem) buf[u.id >> 7]|= u128(1) << (u.id & 127);
    for (auto u: nrem) {
     int b= u.id * m, i= 0, cnt= 0;
     for (u128 tmp; i < m; ++i) tmp= buf[i] & adj[b + i], cnt+= __builtin_popcountll(tmp >> 64) + __builtin_popcountll(u64(tmp));
     deg[u.id]= cnt;
    }
    cur.push_back(p.id), dfs(dfs, nrem), cur.pop_back(), rem.pop_back();
   }
  };
  std::vector<id_num> nrem;
  for (u16 u= n, cnt; u--; nrem.push_back(id_num{u, 0}), deg[u]= cnt) {
   int b= u * m, i= cnt= 0;
   for (u128 tmp; i < m; ++i) tmp= adj[b + i], cnt+= __builtin_popcountll(tmp >> 64) + __builtin_popcountll(u64(tmp));
  }
  return dfs(dfs, nrem), clique;
 }
public:
 CliqueProblem(int n): n(n), m((n + 127) >> 7), adj_(n * m) {}
 void add_edge(int u, int v) { adj_[u * m + (v >> 7)]|= u128(1) << (v & 127), adj_[v * m + (u >> 7)]|= u128(1) << (u & 127); }
 std::vector<u16> get_max_clique() const { return calc(false); }
 std::vector<u16> get_max_independent_set() const { return calc(true); }
 std::vector<u16> get_min_vertex_cover() const {
  std::vector<u128> buf(m);
  for (u16 u: calc(true)) buf[u >> 7]|= u128(1) << (u & 127);
  std::vector<u16> ret;
  for (u16 i= 0; i < n; ++i)
   if (!((buf[i >> 7] >> (i & 127)) & 1)) ret.push_back(i);
  return ret;
 }
 template <class F> void enumerate_cliques(const F &out) const {
  std::vector<u128> buf;
  std::vector<u16> deg(n), clique, nbd;
  for (u16 u= n, cnt; u--; deg[u]= cnt) {
   int b= u * m, i= cnt= 0;
   for (u128 tmp; i < m; ++i) tmp= adj_[b + i], cnt+= __builtin_popcountll(tmp >> 64) + __builtin_popcountll(u64(tmp));
  }
  u16 nn;
  auto dfs= [&](auto dfs, u16 k) -> void {
   out(clique);
   for (u16 i= k; i < nn; ++i) {
    u16 v= nbd[i];
    auto b= adj_.cbegin() + v * m;
    bool all= 1;
    for (u16 j= 0; j < m; ++j) all&= (b[j] & buf[j]) == buf[j];
    if (all) clique.push_back(v), buf[v >> 7]|= u128(1) << (v & 127), dfs(dfs, i + 1), clique.pop_back(), buf[v >> 7]^= u128(1) << (v & 127);
   }
  };
  bool unused[n];
  std::fill_n(unused, n, 1);
  for (u16 _= n; _--;) {
   u16 v, min_d= n;
   for (u16 i= n; i--;)
    if (unused[i] && min_d > deg[i]) v= i, min_d= deg[i];
   nbd.clear(), clique= {v}, buf.assign(m, 0), buf[v >> 7]|= u128(1) << (v & 127);
   auto a= adj_.cbegin() + v * m;
   for (int i= 0; i < n; ++i)
    if ((a[i >> 7] >> (i & 127)) & unused[i]) nbd.push_back(i);
   nn= nbd.size(), dfs(dfs, 0), unused[v]= 0;
   for (auto u: nbd) --deg[u];
  }
 }
};