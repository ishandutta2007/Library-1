#pragma once
#include "src/Math/SetPowerSeries.hpp"
template <unsigned short MAX_V= 21> class UndirectedGraphSetPowerSeries {
 using SPS= SetPowerSeries<MAX_V>;
 template <class T> using sps= std::vector<T>;
 template <class T> using poly= std::vector<T>;
 const unsigned V, sz;
 unsigned adj[MAX_V][MAX_V]= {0}, edge[MAX_V]= {0};
 template <class T> static inline T pow(T x, int k) {
  for (T ret(1);; x*= x)
   if (k & 1 ? ret*= x : 0; !(k>>= 1)) return ret;
 }
 template <class F> inline void bfs(int s, const F &f) const {
  for (int t= s, u, j; t;)
   for (f(u= 1 << __builtin_ctz(t)); u;) j= __builtin_ctz(u), t^= 1 << j, u^= 1 << j, u|= edge[j] & t;
 }
 template <class T, class G> static inline void transform_articulation(sps<T> &f, const G &g) {
  const int sz2= f.size() / 2;
  sps<T> tmp(sz2);
  for (int I= sz2; I; I>>= 1) {
   for (int t= 0; t < sz2; t+= I)
    for (int u= I, t2= t << 1; u--;) tmp[t | u]= f[t2 | I | u];
   tmp= g(tmp);
   for (int t= 0; t < sz2; t+= I)
    for (int u= I, t2= t << 1; u--;) f[t2 | I | u]= tmp[t | u];
  }
 }
 template <class T, bool b> inline void transform_bridge(sps<T> &f) const {
  const int sz2= sz / 2;
  sps<T> tmp(sz2), tmp2;
  for (int i= V, I= sz2; --i; I>>= 1) {
   for (int t= 0; t < sz2; t+= I)
    for (int u= I, t2= t << 1; u--;) tmp[t | u]= f[t2 | I | u];
   tmp2.assign(sz2, 0);
   for (int t= 0; t < sz2; t+= I)
    for (int j= i, J= I, t2= t << 1; J>>= 1, j--;)
     for (int s= J, J2= J * 2; s < I; s+= J2)
      for (int u= s + J; u-- > s;) {
       if constexpr (b) tmp2[t | u]+= f[t2 | u] * adj[i][j];
       else tmp2[t | u]-= f[t2 | u] * adj[i][j];
      }
   tmp= SPS::convolve(tmp, SPS::exp(tmp2));
   for (int t= 0; t < sz2; t+= I)
    for (int u= I, t2= t << 1; u--;) f[t2 | I | u]= tmp[t | u];
  }
 }
public:
 UndirectedGraphSetPowerSeries(int n): V(n), sz(1 << V) {}
 UndirectedGraphSetPowerSeries(const std::vector<std::vector<int>> &g): V(g.size()), sz(1 << V) {
  for (int i= V; i--;)
   for (int j= i; j--;) assert(g[i][j] == g[j][i]);
  for (int i= V; i--;)
   for (int j= V; j--;) adj[i][j]= g[i][j];
  for (int i= V; i--;)
   for (int j= V; j--;) edge[i]|= !(!(adj[i][j])) << j;
 }
 void add_edge(int u, int v, int cnt= 1) {
  adj[u][v]= (adj[v][u]+= cnt), edge[u]|= (1 << v), edge[v]|= (1 << u);
  if (!(adj[u][v])) edge[u]^= (1 << v), edge[v]^= (1 << u);
 }
 const unsigned *operator[](int u) const { return adj[u]; }
 template <class T> static inline sps<T> only_connected(const sps<T> &f) { SPS::log(f); }
 template <class T> static inline sps<T> disjoint_union(const sps<T> &f) { SPS::exp(f); }
 template <class T> static inline sps<T> only_biconnected(sps<T> f) { return transform_articulation(f, SPS::template log<T>), f; }
 template <class T> static inline sps<T> articulation_union(sps<T> f) { return transform_articulation(f, SPS::template exp<T>), f; }
 template <class T> inline sps<T> only_2edge_connected(sps<T> f) const { return transform_bridge<T, false>(f), f; }
 template <class T> inline sps<T> bridge_union(sps<T> f) const { return transform_bridge<T, true>(f), f; }
 inline sps<int> edge_num() const {
  sps<int> ret(sz, 0);
  for (int i= V; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]= adj[i][j];
  return SPS::subset_sum(ret), ret;
 }
 inline sps<int> connected_component_num() const {
  sps<int> ret(sz, 0);
  for (int s= sz; s--;) bfs(s, [&](int) { ret[s]++; });
  return ret;
 }
 inline sps<int> cycle_space_rank() const {
  sps<int> e= edge_num(), k= connected_component_num(), ret(sz, 0);
  for (int s= sz; s--;) ret[s]= e[s] + k[s] - __builtin_popcount(s);
  return ret;
 }
 template <class T> inline void selfloop_num(sps<T> &f) const {
  sps<int> ret(sz, 0);
  for (int i= V; i--;) ret[1 << i]= adj[i][i];
  return SPS::subset_sum(ret), ret;
 }
 template <class T> static inline sps<T> space_size(const sps<int> &rank) {
  sps<T> ret(rank.size());
  for (int s= rank.size(); s--;) ret[s]= pow<T>(2, rank[s]);
  return ret;
 }
 template <class T> inline sps<T> graph() const { return space_size<T>(edge_num()); }
 template <class T> inline sps<T> cycle_space_size() const { return space_size<T>(cycle_space_rank()); }
 template <class T> inline sps<T> connected_graph() const { return SPS::log(graph<T>()); }
 template <class T> inline sps<T> eulerian_graph() const { return SPS::log(cycle_space_size<T>()); }
 template <class T> inline sps<T> connected_biparate_graph() const {
  sps<T> tmp= graph<T>(), ret(sz, 1);
  for (int s= sz; s--;) ret[s]/= tmp[s];
  ret= SPS::convolve(ret, ret);
  for (int s= sz; s--;) ret[s]*= tmp[s];
  ret= SPS::log(ret);
  for (int s= sz; s--;) ret[s]/= 2;
  return ret;
 }
 template <class T> inline sps<T> tree() const {
  sps<int> e= edge_num();
  sps<T> ret= {0, 1};
  ret.reserve(sz);
  for (int I= 2; I < sz; I<<= 1) {
   sps<T> g(ret);
   for (int s= I; --s;) g[s]*= e[s | I] - e[s] - e[I];
   g= SPS::exp(g), std::copy(g.begin(), g.end(), std::back_inserter(ret));
  }
  return ret;
 }
 template <class T> inline sps<T> forest() const { return SPS::exp(tree<T>()); }
 template <class T> inline sps<T> cycle_graph() const {
  T dp[sz][V - 1];
  sps<T> ret(sz, 0);
  for (int i= V, I= sz; I>>= 1, --i;) {
   for (int s= I; --s;) std::fill_n(dp[s], i, 0);
   for (int j= i; j--;) dp[1 << j][j]= adj[i][j];
   for (int s= 1; s < I; ++s)
    for (int t= s, j, u, r, k; t; ret[s | I]+= dp[s][j] * adj[j][i])
     for (t^= 1 << (j= __builtin_ctz(t)), u= r= s ^ (1 << j); u; dp[s][j]+= dp[r][k] * adj[k][j]) u^= 1 << (k= __builtin_ctz(u));
  }
  for (int i= V; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]-= adj[i][j];
  for (int s= sz; --s;) ret[s]/= 2;
  return ret;
 }
 template <class T> inline sps<T> biconnected_graph() const {
  sps<T> ret= connected_graph<T>();
  return transform_articulation(ret, SPS::template log<T>), ret;
 }
 template <class T> inline sps<T> two_edge_connected_graph() const {
  sps<T> ret= connected_graph<T>();
  return transform_bridge<T, false>(ret), ret;
 }
 template <class T> inline sps<T> cactus_graph() const {
  auto ret= cycle_graph<T>();
  for (int i= V; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]+= adj[i][j];
  return transform_articulation(ret, SPS::template exp<T>), ret;
 }
 template <class T> inline sps<T> acyclic_orientations() const {
  auto k= connected_component_num();
  sps<T> g(sz, 0);
  for (int s= sz; --s;)
   if (k[s] == __builtin_popcount(s)) g[s]= k[s] & 1 ? 1 : -1;
  return SPS::template semi_relaxed_convolve<T>(g, 1);
 }
 template <class T> inline std::vector<T> colorings_using_exactly_k_colors_num() const {
  if (V == 0) return {0};  // impossible in any number of ways
  auto k= connected_component_num();
  std::vector<T> indep(sz, 0);
  for (int s= sz; --s;) indep[s]= k[s] == __builtin_popcount(s);
  return SPS::egf(indep);
 }
 template <class T> inline poly<T> chromatic_polynomial() const {
  auto e= colorings_using_exactly_k_colors_num<T>();
  if (e.back() == 0) return {0};
  poly<T> ret(V + 1, 0);
  T tmp[V]= {1};
  for (int i= 1, j; i < V; ++i)
   for (j= i; j--; tmp[j]*= -i) ret[j + 1]+= tmp[j] * e[i], tmp[j + 1]+= tmp[j];
  for (int j= V; j--;) ret[j + 1]+= tmp[j];
  return ret;
 }
 template <class T> inline T tutte_polynomial(T x, T y) const {
  int sum[sz], s, t, lim= 2, i, j;
  T fum[10'000]= {0, 1};
  std::vector<T> g= {0}, h;
  for (g.reserve(sz), h.reserve(sz), i= 0; i < V; ++i) {
   for (sum[0]= j= 0; j < i; j++)
    for (s= t= 1 << j; s--;) sum[s | t]= sum[s] + adj[i][j];
   for (h.resize(s= 1 << i); s--; h[s]= g[s] * fum[sum[s]])
    for (; lim <= sum[s]; lim++) fum[lim]= fum[lim - 1] * y + 1;
   h= SPS::exp(h), std::copy(h.begin(), h.end(), std::back_inserter(g));
  }
  for (x-= 1, t= ~0, j= 0, i= V; i--;) j+= adj[i][i];
  for (bfs((s= sz) - 1, [&](int u) { t^= u; }); --s&= t;) g[s]*= x;
  return SPS::exp(g)[sz - 1] * pow(y, j);
 }
};