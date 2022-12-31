#pragma once
#include <bits/stdc++.h>
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
 int *operator[](int u) const { return adj[u]; }
 void add_edge(int u, int v, int cnt= 1) {
  adj[u][v]= (adj[v][u]+= cnt), edge[u]|= (1 << v), edge[v]|= (1 << u);
  if (!(adj[u][v])) edge[u]^= (1 << v), edge[v]^= (1 << u);
 }
 template <class T> static inline sps<T> space_size(const sps<int> &rank) {
  sps<T> ret(rank.size());
  for (int s= rank.size(); s--;) ret[s]= pow<T>(2, rank[s]);
  return ret;
 }
 template <class T, class G> static inline void transform(sps<T> &f, const G &g) {
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
 template <class T>  // O(V^3 2^V)
 static inline void connect_to_biconnect(sps<T> &f) {
  transform(f, SPS::template log<T>);
 }
 template <class T>  // O(V^3 2^V)
 static inline void biconnect_to_connect(sps<T> &f) {
  transform(f, SPS::template exp<T>);
 }
 template <class T>  // O(V 2^V)
 inline void loop_ignored_to_loop_permitted(sps<T> &f) const {
  auto tmp= space_size<T>(loop_size());
  for (int s= sz; s--;) f[s]*= tmp[s];
 }
 inline sps<int> edge_space_rank() const {  // O(V 2^V)
  sps<int> ret(sz, 0);
  for (int i= V; i--;)
   for (int j= i + 1; j--;) ret[(1 << i) | (1 << j)]= adj[i][j];
  return SPS::subset_sum(ret), ret;
 }
 inline sps<int> loop_size() const {  // O(V 2^V)
  sps<int> ret(sz, 0);
  for (int i= V; i--;) ret[(1 << i)]= adj[i][i];
  return SPS::subset_sum(ret), ret;
 }
 inline sps<int> loop_ignored_edge_space_rank() const {  // O(V 2^V)
  sps<int> ret(sz, 0);
  for (int i= V; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]= adj[i][j];
  return SPS::subset_sum(ret), ret;
 }
 inline sps<int> connected_component_num() const {  // O(V 2^V)
  sps<int> ret(sz, 0);
  for (int s= sz; s--;) bfs(s, [&](int) { ret[s]++; });
  return ret;
 }
 inline sps<int> cycle_space_rank() const {  // O(V 2^V)
  sps<int> e= edge_space_rank(), k= connected_component_num(), ret(sz, 0);
  for (int s= sz; s--;) ret[s]= e[s] + k[s] - __builtin_popcount(s);
  return ret;
 }
 template <class T>  // O(V 2^V)
 inline sps<T> edge_space_size() const {
  return space_size<T>(edge_space_rank());
 }
 template <class T>  // O(V 2^V)
 inline sps<T> loop_ignored_edge_space_size() const {
  return space_size<T>(loop_ignored_edge_space_rank());
 }
 template <class T>  // O(V 2^V)
 inline sps<T> cycle_space_size() const {
  return space_size<T>(cycle_space_rank());
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> connected_graph_num() const {
  return SPS::log(edge_space_size<T>());
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> loop_ignored_connected_graph_num() const {
  return SPS::log(loop_ignored_edge_space_size<T>());
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> euler_graph_num() const {
  return SPS::log(cycle_space_size<T>());
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> connected_biparate_graph_num() const {
  sps<T> tmp= edge_space_size<T>(), ret(sz, 1);
  for (int s= sz; s--;) ret[s]/= tmp[s];
  ret= SPS::convolve(ret, ret);
  for (int s= sz; s--;) ret[s]*= tmp[s];
  ret= SPS::log(ret);
  for (int s= sz; s--;) ret[s]/= 2;
  return ret;
 }
 template <class T>  // O(V^3 2^V)
 inline sps<T> loop_ignored_biconnected_graph_num() const {
  auto ret= loop_ignored_connected_graph_num<T>();
  return connect_to_biconnect(ret), ret;
 }
 template <class T>  // O(V^3 2^V)
 inline sps<T> biconnected_graph_num() const {
  auto ret= loop_ignored_biconnected_graph_num<T>();
  return loop_ignored_to_loop_permitted(ret), ret;
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> spanning_tree_num() const {
  sps<int> e= edge_space_rank();
  sps<T> ret= {0, 1};
  ret.reserve(sz);
  for (int I= 2; I < sz; I<<= 1) {
   sps<T> g(ret);
   for (int s= I; --s;) g[s]*= e[s | I] - e[s] - e[I];
   g= SPS::exp(g);
   std::copy(g.begin(), g.end(), std::back_inserter(ret));
  }
  return ret;
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> forest_num() const {
  return SPS::exp(spanning_tree_num<T>());
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> rooted_spanning_tree_num() const {
  auto ret= spanning_tree_num<T>();
  for (int s= sz; s--;) ret[s]*= __builtin_popcount(s);
  return ret;
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> rooted_forest_num() const {
  return SPS::exp(rooted_spanning_tree_num<T>());
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> cycle_graph_num() const {
  T dp[sz][V - 1];
  sps<T> ret(sz, 0);
  for (int i= V, I= sz; I>>= 1, --i;) {
   for (int s= I; --s;) std::fill_n(dp[s], i, 0);
   for (int j= i; j--;) dp[1 << j][j]= adj[i][j];
   for (int s= 1; s < I; s++)
    for (int t= s, j, u, r, k; t; ret[s | I]+= dp[s][j] * adj[j][i])
     for (t^= 1 << (j= __builtin_ctz(t)), u= r= s ^ (1 << j); u; dp[s][j]+= dp[r][k] * adj[k][j]) u^= 1 << (k= __builtin_ctz(u));
  }
  for (int i= V; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]-= adj[i][j];
  for (int s= sz; --s;) ret[s]/= 2;
  for (int i= V; i--;) ret[1 << i]= adj[i][i];
  return ret;
 }
 template <class T>  // O(V^3 2^V)
 inline sps<T> cactus_graph_num() const {
  auto ret= cycle_graph_num<T>();
  for (int i= V; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]+= adj[i][j];
  for (int i= V; i--;) ret[1 << i]= 0;
  return biconnect_to_connect(ret), loop_ignored_to_loop_permitted(ret), ret;
 }
 template <class T>  // O(V^3 2^V)
 inline sps<T> two_edge_connected_graph_num() const {
  const int sz2= sz / 2;
  sps<T> ret= connected_graph_num<T>(), tmp(sz2), tmp2;
  for (int i= V, I= sz2; --i; I>>= 1) {
   for (int t= 0; t < sz2; t+= I)
    for (int u= I, t2= t << 1; u--;) tmp[t | u]= ret[t2 | I | u];
   tmp2.assign(sz2, 0);
   for (int t= 0; t < sz2; t+= I)
    for (int j= i, J= I, t2= t << 1; J>>= 1, j--;)
     for (int s= J, J2= J * 2; s < I; s+= J2)
      for (int u= s + J; u-- > s;) tmp2[t | u]-= ret[t2 | u] * adj[i][j];
   tmp= SPS::convolve(tmp, SPS::exp(tmp2));
   for (int t= 0; t < sz2; t+= I)
    for (int u= I, t2= t << 1; u--;) ret[t2 | I | u]= tmp[t | u];
  }
  return ret;
 }
 template <class T>  // O(V^2 2^V)
 inline sps<T> acyclic_orientations() const {
  auto k= connected_component_num();
  sps<T> g(sz, 0);
  for (int s= sz; --s;)
   if (k[s] == __builtin_popcount(s)) g[s]= (k[s] + 1) & 1 ? -1 : 1;
  return SPS::template online_convolve<T>(g, 1);
 }
 template <class T>  // O(V^2 2^V)
 inline std::vector<T> colorings_using_exactly_k_colors_num() const {
  if (V == 0) return {0};  // impossible in any number of ways
  for (int i= V; i--;)
   if (adj[i][i]) return {0};  // impossible in any number of ways
  auto k= connected_component_num();
  std::vector<T> indep(sz, 0);
  for (int s= sz; --s;) indep[s]= k[s] == __builtin_popcount(s);
  return SPS::egf(indep);
 }
 template <class T>  // O(V^2 2^V)
 inline poly<T> chromatic_polynomial() const {
  auto e= colorings_using_exactly_k_colors_num<T>();
  if (e.back() == 0) return {0};
  poly<T> ret(V + 1, 0);
  T tmp[V]= {1};
  for (int i= 1, j; i < V; i++)
   for (j= i; j--; tmp[j]*= -i) ret[j + 1]+= tmp[j] * e[i], tmp[j + 1]+= tmp[j];
  for (int j= V; j--;) ret[j + 1]+= tmp[j];
  return ret;
 }
 template <class T>  //  O(V^2 2^V)
 inline T tutte_polynomial(T x, T y) const {
  int sum[sz], s, t, lim= 2, i, j;
  T fum[10'000]= {0, 1};
  std::vector<T> g= {0}, h;
  for (g.reserve(sz), h.reserve(sz), i= 0; i < V; i++) {
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