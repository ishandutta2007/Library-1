#pragma once
#include <limits>
#include <iterator>
#include <vector>
#include <queue>
#include <algorithm>
template <class cost_t, bool min_perfect= false> class WeightedMatching {
 static constexpr cost_t INF= std::numeric_limits<cost_t>::max() / 4;
 struct E {
  int16_t u, v;
  cost_t w;
 };
 template <class T> struct Mat {
  int n;
  std::vector<T> dat;
  Mat(int n): n(n), dat(n * n){};
  auto operator[](int i) { return std::next(dat.begin(), n * i); }
  const auto operator[](int i) const { return std::next(dat.cbegin(), n * i); }
 };
 cost_t mx;
 Mat<cost_t> adj;
 std::vector<int16_t> mt;
 static inline int sgn(cost_t c) {
  static constexpr cost_t EPS= 1e-10;
  return c < -EPS ? -1 : c > EPS ? 1 : 0;
 }
public:
 WeightedMatching(int n): mx(0), adj(n), mt(2 * n) {
  if constexpr (min_perfect)
   for (int i= n; i--;)
    for (int j= n; j--;) adj[i][j]= INF;
 }
 void add_edge(int u, int v, cost_t w) {
  mx= std::max(mx, w);
  if constexpr (min_perfect) {
   if (adj[u][v] > w) adj[u][v]= adj[v][u]= w;
  } else if (adj[u][v] < w) adj[u][v]= adj[v][u]= w;
 }
 void build() {
  const int n= mt.size() / 2;
  int16_t rt[2 * n], used[2 * n], in= 0, m= n, q[2 * n], slk[2 * n], par[2 * n], isS[2 * n];
  Mat<int16_t> blg(2 * n);
  std::fill_n(used, 2 * n, 0), std::fill_n(rt + n + 1, n - 1, 0), rt[0]= 0;
  std::vector<std::vector<int16_t>> fwr(2 * n);
  std::queue<int16_t> que;
  std::vector<cost_t> dual(2 * n);
  Mat<E> G(2 * n);
  for (int16_t i= 0; i <= n; ++i) G[i][0]= E{i, int16_t(0), 0};
  for (int16_t j= 0; j <= n; ++j) G[0][j]= E{int16_t(0), j, 0};
  if constexpr (min_perfect)
   for (int16_t i= 1; i <= n; ++i)
    for (int16_t j= 1; j <= n; ++j) G[i][j]= E{i, j, adj[i - 1][j - 1] == INF ? 0 : (mx - adj[i - 1][j - 1] + 1) * 2};
  else
   for (int16_t i= 1; i <= n; ++i)
    for (int16_t j= 1; j <= n; ++j) G[i][j]= E{i, j, adj[i - 1][j - 1] * 2};
  cost_t inf= 0;
  for (int16_t i= 1; i <= n; ++i)
   for (int16_t j= 1; j <= n; ++j) inf= std::max(inf, G[i][j].w);
  inf+= 1;
  for (int i= 1; i <= n; i++) rt[i]= i, blg[i][i]= i, dual[i]= inf;
  auto dist= [&](const E &e) { return dual[e.u] - e.w + dual[e.v]; };
  auto push= [&](int v) {
   q[0]= v;
   for (int i= 0, s= 1; i < s; ++i) {
    if (v= q[i]; v > n)
     for (int u: fwr[v]) q[s++]= u;
    else que.push(v);
   }
  };
  auto match= [&](auto self, int u, int v) -> void {
   if (mt[u]= G[u][v].v; u <= n) return;
   int x= blg[u][G[u][v].u], p= std::find(fwr[u].begin(), fwr[u].end(), x) - fwr[u].begin();
   if (p & 1) std::reverse(fwr[u].begin() + 1, fwr[u].end()), p= fwr[u].size() - p;
   for (int i= 0; i < p; ++i) self(self, fwr[u][i], fwr[u][i ^ 1]);
   self(self, x, v), std::rotate(fwr[u].begin(), fwr[u].begin() + p, fwr[u].end());
  };
  auto path= [&](const E &e) {
   if (int u= rt[e.u], v= rt[e.v], bu= u, bv= v, x; isS[v] == 1) {
    for (++in; bu; bu= rt[mt[bu]] ? rt[par[rt[mt[bu]]]] : 0) used[bu]= in;
    for (int id= n + 1; bv; bv= rt[mt[bv]] ? rt[par[rt[mt[bv]]]] : 0)
     if (used[bv] == in) {
      while (id <= m && rt[id]) ++id;
      for (int i= (m+= (id > m)); i; --i) G[id][i].w= G[i][id].w= 0;
      for (int i= n; --i;) blg[id][i]= 0;
      mt[id]= mt[bv];
      for (fwr[id].clear(); u != bv; fwr[id].push_back(u), u= rt[par[u]]) fwr[id].push_back(u), push(u= rt[mt[u]]);
      fwr[id].push_back(bv), std::reverse(fwr[id].begin(), fwr[id].end());
      for (; v != bv; fwr[id].push_back(v), v= rt[par[v]]) fwr[id].push_back(v), push(v= rt[mt[v]]);
      isS[id]= 1, dual[id]= 0, q[0]= id;
      for (int i= 0, s= 1; i < s; ++i)
       if (rt[v= q[i]]= id; v > n)
        for (int t: fwr[v]) q[s++]= t;
      for (int c: fwr[id]) {
       for (int i= 1; i <= m; ++i)
        if (sgn(G[id][i].w) == 0 || sgn(dist(G[c][i]) - dist(G[id][i])) < 0) G[id][i]= G[c][i], G[i][id]= G[i][c];
       for (int i= 1; i <= n; ++i)
        if (blg[c][i]) blg[id][i]= c;
      }
      for (int i= slk[id]= 0; i <= n; ++i)
       if (sgn(G[i][id].w) != 0 && rt[i] != id && isS[rt[i]] == 1)
        if (!slk[id] || sgn(dist(G[i][id]) - dist(G[slk[id]][id])) < 0) slk[id]= i;
      return false;
     }
    for (bu= u, bv= v;; match(match, bv= x, bu= rt[par[x]]))
     if (x= rt[mt[bu]], match(match, bu, bv); !x)
      for (;; match(match, u= x, v= rt[par[x]]))
       if (x= rt[mt[v]], match(match, v, u); !x) return true;
   } else if (!isS[v]) par[v]= e.u, isS[v]= 2, slk[v]= slk[x= rt[mt[v]]]= 0, isS[x]= 1, push(x);
   return false;
  };
 LABEL_AUGMENT:
  std::fill_n(isS, 2 * n, 0), std::fill_n(slk, 2 * n, 0), std::fill_n(par, 2 * n, 0), que= std::queue<int16_t>();
  for (int i= 1; i <= m; i++)
   if (rt[i] == i && !mt[i]) isS[i]= 1, push(i);
  if (que.empty()) return;
  for (cost_t del= inf;; del= inf) {
   for (int v, i; !que.empty();)
    for (v= que.front(), que.pop(), i= 1; i <= n; ++i)
     if (sgn(G[v][i].w) != 0 && rt[i] != rt[v]) {
      if (sgn(dist(G[v][i])) == 0) {
       if (path(G[v][i])) goto LABEL_AUGMENT;
      } else if (isS[rt[i]] != 2)
       if (!slk[rt[i]] || sgn(dist(G[v][rt[i]]) - dist(G[slk[rt[i]]][rt[i]])) < 0) slk[rt[i]]= v;
     }
   for (int i= n + 1; i <= m; ++i)
    if (rt[i] == i && isS[i] == 2 && sgn(del - dual[i] / 2) > 0) del= dual[i] / 2;
   for (int i= 1; i <= m; ++i)
    if (rt[i] == i && slk[i] && isS[i] != 2)
     if (cost_t c= dist(G[slk[i]][i]) / (1 + isS[i]); sgn(del - c) > 0) del= c;
   for (int i= 1; i <= n; ++i) {
    if (isS[rt[i]] == 1) {
     if (sgn(dual[i]-= del) <= 0) return;
    } else if (isS[rt[i]] == 2) dual[i]+= del;
   }
   for (int i= n + 1; i <= m; ++i)
    if (rt[i] == i && isS[i]) dual[i]+= isS[i] == 1 ? 2 * del : -2 * del;
   for (int i= 1; i <= m; ++i)
    if (rt[i] == i && slk[i] && rt[slk[i]] != i && sgn(dist(G[slk[i]][i])) == 0)
     if (path(G[slk[i]][i])) goto LABEL_AUGMENT;
   for (int b= n + 1, x, p, T, S; b <= m; b++)
    if (rt[b] == b && isS[b] == 2 && sgn(dual[b]) == 0) {
     isS[x= blg[b][G[b][par[b]].u]]= 2, par[x]= par[b];
     for (int c: fwr[b]) {
      q[0]= c;
      for (int i= 0, s= 1, v; i < s; ++i)
       if (rt[v= q[i]]= c; v > n)
        for (int u: fwr[v]) q[s++]= u;
     }
     p= std::find(fwr[b].begin(), fwr[b].end(), x) - fwr[b].begin(), x= fwr[b].size();
     if (p & 1) std::reverse(fwr[b].begin() + 1, fwr[b].end()), p= fwr[b].size() - p;
     for (int i= 0; i < p; i+= 2) isS[S= fwr[b][i + 1]]= 1, isS[T= fwr[b][i]]= 2, par[T]= G[S][T].u, slk[S]= slk[T]= 0, push(S);
     for (int i= p + 1, j; i < x; ++i)
      for (T= fwr[b][i], j= isS[T]= slk[T]= 0; j <= n; ++j)
       if (sgn(G[j][T].w) != 0 && rt[j] != T && isS[rt[j]] == 1)
        if (!slk[T] || sgn(dist(G[j][T]) - dist(G[slk[T]][T])) < 0) slk[T]= j;
     fwr[b].clear(), rt[b]= 0;
    }
  }
  goto LABEL_AUGMENT;
 }
 int match(int v) const { return mt[v + 1] - 1; }
 std::vector<E> weight_matching() const {
  const int n= mt.size() / 2;
  std::vector<E> ret;
  if constexpr (min_perfect) {
   if (n & 1) return {};  // no solution
   for (int16_t i= 0; i < n; ++i)
    if (int16_t j= match(i); i < j) ret.push_back(E{i, j, adj[i][j]});
   if (2 * int(ret.size()) != n) return {};  // no solution
  } else
   for (int16_t i= 0; i < n; ++i)
    if (int16_t j= match(i); i < j) ret.push_back(E{i, j, adj[i][j]});
  return ret;
 }
};