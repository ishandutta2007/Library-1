#pragma once
#include "src/Math/set_power_series.hpp"
class UndirectedGraphSetPowerSeries {
 template <class T> using Sps= std::vector<T>;
 template <class T> using Poly= std::vector<T>;
 const int n, N;
 std::vector<int> adj, es;
 template <class T> static inline T pow(T x, int k) {
  for (T ret(1);; x*= x)
   if (k& 1 ? ret*= x : 0; !(k>>= 1)) return ret;
 }
 template <class F> inline void bfs(int s, const F& f) const {
  for (int t= s, u, j; t;)
   for (f(u= 1 << __builtin_ctz(t)); u;) j= __builtin_ctz(u), t^= 1 << j, u^= 1 << j, u|= es[j] & t;
 }
 template <class T, class G> static inline void transform_articulation(Sps<T>& f, const G& g) {
  const int M= f.size() / 2;
  Sps<T> tmp(M);
  for (int I= M; I; I>>= 1) {
   for (int t= 0; t < M; t+= I)
    for (int u= I, t2= t << 1; u--;) tmp[t | u]= f[t2 | I | u];
   tmp= g(tmp);
   for (int t= 0; t < M; t+= I)
    for (int u= I, t2= t << 1; u--;) f[t2 | I | u]= tmp[t | u];
  }
 }
 template <class T, bool b> inline void transform_bridge(Sps<T>& f) const {
  const int M= N / 2;
  Sps<T> tmp(M), tmp2;
  for (int i= n, I= M; --i; I>>= 1) {
   for (int t= 0; t < M; t+= I)
    for (int u= I, t2= t << 1; u--;) tmp[t | u]= f[t2 | I | u];
   tmp2.assign(M, 0);
   for (int t= 0; t < M; t+= I)
    for (int j= i, J= I, t2= t << 1; J>>= 1, j--;)
     for (int s= J, J2= J * 2; s < I; s+= J2)
      for (int u= s + J; u-- > s;) {
       if constexpr (b) tmp2[t | u]+= f[t2 | u] * adj[i * n + j];
       else tmp2[t | u]-= f[t2 | u] * adj[i * n + j];
      }
   tmp= sps::convolve(tmp, sps::exp(tmp2));
   for (int t= 0; t < M; t+= I)
    for (int u= I, t2= t << 1; u--;) f[t2 | I | u]= tmp[t | u];
  }
 }
public:
 UndirectedGraphSetPowerSeries(int n): n(n), N(1 << n), adj(n * n), es(n) {}
 UndirectedGraphSetPowerSeries(const std::vector<std::vector<int>>& g): n(g.size()), N(1 << n), adj(n * n), es(n) {
  for (int i= n; i--;)
   for (int j= i; j--;) assert(g[i][j] == g[j][i]);
  for (int i= n; i--;)
   for (int j= n; j--;) adj[i * n + j]= g[i][j];
  for (int i= n; i--;)
   for (int j= n; j--;) es[i]|= !(!(adj[i * n + j])) << j;
 }
 void add_edge(int u, int v, int cnt= 1) {
  adj[u * n + v]= (adj[v * n + u]+= cnt), es[u]|= (1 << v), es[v]|= (1 << u);
  if (!(adj[u * n + v])) es[u]^= (1 << v), es[v]^= (1 << u);
 }
 const auto operator[](int u) const { return adj.begin() + (u * n); }
 template <class T> static inline Sps<T> only_connected(const Sps<T>& f) { return sps::log(f); }
 template <class T> static inline Sps<T> disjoint_union(const Sps<T>& f) { return sps::exp(f); }
 template <class T> static inline Sps<T> only_biconnected(Sps<T> f) { return transform_articulation(f, sps::log<T>), f; }
 template <class T> static inline Sps<T> articulation_union(Sps<T> f) { return transform_articulation(f, sps::exp<T>), f; }
 template <class T> inline Sps<T> only_2edge_connected(Sps<T> f) const { return transform_bridge<T, false>(f), f; }
 template <class T> inline Sps<T> bridge_union(Sps<T> f) const { return transform_bridge<T, true>(f), f; }
 inline Sps<int> edge_num() const {
  Sps<int> ret(N, 0);
  for (int i= n; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]= adj[i * n + j];
  return sps::subset_zeta(ret), ret;
 }
 inline Sps<int> connected_component_num() const {
  Sps<int> ret(N, 0);
  for (int s= N; s--;) bfs(s, [&](int) { ret[s]++; });
  return ret;
 }
 inline Sps<int> cycle_space_rank() const {
  Sps<int> e= edge_num(), k= connected_component_num(), ret(N, 0);
  for (int s= N; s--;) ret[s]= e[s] + k[s] - __builtin_popcount(s);
  return ret;
 }
 inline Sps<int> odd_deg_num() const {
  Sps<int> ret(N, 0);
  for (int i= n, I= N; I>>= 1, i--;)
   for (int t= 0, I2= I << 1; t < N; t+= I2)
    for (int u= I, cnt, v, j; u--; ret[t | I | u]+= cnt & 1)
     for (cnt= 0, v= t | u; v; v^= 1 << j) cnt+= adj[i * n + (j= __builtin_ctz(v))];
  return ret;
 }
 inline Sps<int> selfloop_num() const {
  Sps<int> ret(N, 0);
  for (int i= 0, I= 1; i < n; ++i, I<<= 1)
   for (int u= I; u--;) ret[I | u]= ret[u] + adj[i * n + i];
  return ret;
 }
 template <class T> static inline Sps<T> space_size(const Sps<int>& rank) {
  Sps<T> ret(rank.size());
  for (int s= rank.size(); s--;) ret[s]= pow<T>(2, rank[s]);
  return ret;
 }
 template <class T> inline Sps<T> graph() const { return space_size<T>(edge_num()); }
 template <class T> inline Sps<T> cycle_space_size() const { return space_size<T>(cycle_space_rank()); }
 template <class T> inline Sps<T> connected_graph() const { return sps::log(graph<T>()); }
 template <class T> inline Sps<T> eulerian_graph() const { return sps::log(cycle_space_size<T>()); }
 template <class T> inline Sps<T> connected_biparate_graph() const {
  Sps<T> tmp= graph<T>(), ret(N, 1);
  for (int s= N; s--;) ret[s]/= tmp[s];
  ret= sps::convolve(ret, ret);
  for (int s= N; s--;) ret[s]*= tmp[s];
  ret= sps::log(ret);
  for (int s= N; s--;) ret[s]/= 2;
  return ret;
 }
 template <class T> inline Sps<T> tree() const {
  Sps<int> e= edge_num();
  Sps<T> ret= {0, 1};
  ret.reserve(N);
  for (int I= 2; I < N; I<<= 1) {
   Sps<T> g(ret);
   for (int s= I; --s;) g[s]*= e[s | I] - e[s] - e[I];
   g= sps::exp(g), std::copy(g.begin(), g.end(), std::back_inserter(ret));
  }
  return ret;
 }
 template <class T> inline Sps<T> forest() const { return sps::exp(tree<T>()); }
 template <class T> inline Sps<T> cycle_graph() const {
  T dp[N][n - 1];
  Sps<T> ret(N, 0);
  for (int i= n, I= N; I>>= 1, --i;) {
   for (int s= I; --s;) std::fill_n(dp[s], i, 0);
   for (int j= i; j--;) dp[1 << j][j]= adj[i * n + j];
   for (int s= 1; s < I; ++s)
    for (int t= s, j, u, r, k; t; ret[s | I]+= dp[s][j] * adj[j * n + i])
     for (t^= 1 << (j= __builtin_ctz(t)), u= r= s ^ (1 << j); u; dp[s][j]+= dp[r][k] * adj[k * n + j]) u^= 1 << (k= __builtin_ctz(u));
  }
  for (int i= n; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]-= adj[i * n + j];
  for (int s= N; --s;) ret[s]/= 2;
  return ret;
 }
 template <class T> inline Sps<T> biconnected_graph() const {
  Sps<T> ret= connected_graph<T>();
  return transform_articulation(ret, sps::log<T>), ret;
 }
 template <class T> inline Sps<T> two_edge_connected_graph() const {
  Sps<T> ret= connected_graph<T>();
  return transform_bridge<T, false>(ret), ret;
 }
 template <class T> inline Sps<T> cactus_graph() const {
  auto ret= cycle_graph<T>();
  for (int i= n; i--;)
   for (int j= i; j--;) ret[(1 << i) | (1 << j)]+= adj[i * n + j];
  return transform_articulation(ret, sps::exp<T>), ret;
 }
 template <class T> inline Sps<T> acyclic_orientations() const {
  auto k= connected_component_num();
  Sps<T> g(N, 0);
  for (int s= N; --s;)
   if (k[s] == __builtin_popcount(s)) g[s]= k[s] & 1 ? -1 : 1;
  return g[0]= 1, sps::inv(g);
 }
 template <class T> inline std::vector<T> colorings_using_exactly_k_colors_num() const {
  if (n == 0) return {0};  // impossible in any number of ways
  auto k= connected_component_num();
  std::vector<T> indep(N, 0);
  for (int s= N; --s;) indep[s]= k[s] == __builtin_popcount(s);
  return sps::egf_T(indep);
 }
 template <class T> inline Poly<T> chromatic_polynomial() const {
  auto e= colorings_using_exactly_k_colors_num<T>();
  if (e.back() == 0) return {0};
  Poly<T> ret(n + 1, 0);
  T tmp[n]= {1};
  for (int i= 1, j; i < n; ++i)
   for (j= i; j--; tmp[j]*= -i) ret[j + 1]+= tmp[j] * e[i], tmp[j + 1]+= tmp[j];
  for (int j= n; j--;) ret[j + 1]+= tmp[j];
  return ret;
 }
 template <class T> inline T tutte_polynomial(T x, T y) const {
  int sum[N], s, t, lim= 2, i, j;
  T fum[10'000]= {0, 1};
  std::vector<T> g= {0}, h;
  for (g.reserve(N), h.reserve(N), i= 0; i < n; ++i) {
   for (sum[0]= j= 0; j < i; j++)
    for (s= t= 1 << j; s--;) sum[s | t]= sum[s] + adj[i * n + j];
   for (h.resize(s= 1 << i); s--; h[s]= g[s] * fum[sum[s]])
    for (; lim <= sum[s]; lim++) fum[lim]= fum[lim - 1] * y + 1;
   h= sps::exp(h), std::copy(h.begin(), h.end(), std::back_inserter(g));
  }
  for (x-= 1, t= ~0, j= 0, i= n; i--;) j+= adj[i * n + i];
  for (bfs((s= N) - 1, [&](int u) { t^= u; }); --s&= t;) g[s]*= x;
  return sps::exp(g)[N - 1] * pow(y, j);
 }
};