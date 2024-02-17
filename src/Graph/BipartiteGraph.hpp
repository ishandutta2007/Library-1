#pragma once
#include <array>
#include <algorithm>
#include <numeric>
#include <cassert>
#include "src/Internal/ListRange.hpp"
class BipartiteGraph {
 std::vector<std::array<int, 2>> es;
 std::vector<int> col, pos, ord, pre, mate, blg;
 CSRArray<int> dag_[2];
 int l;
public:
 BipartiteGraph(int n): col(n, -1), pos(n + 1), ord(n), mate(n, -1), blg(n, -3), l(0) {}
 void add_edge(int u, int v) { es.push_back({u, v}); }
 void build() {
  const int n= col.size();
  std::vector<int> rt, que(n), g(es.size() * 2);
  for (auto [u, v]: es) ++pos[u], ++pos[v];
  for (int i= 0; i < n; ++i) pos[i + 1]+= pos[i];
  for (auto [u, v]: es) g[--pos[u]]= v, g[--pos[v]]= u;
  for (int s= n, i= 0, t= 0, r= n; s--;)
   if (col[s] == -1)
    for (col[s]= 0, ord[l++]= que[t++]= s; i < t; ++i)
     for (int v= que[i], c= col[v], j= pos[v], u; j < pos[v + 1]; ++j)
      if (assert(col[u= g[j]] != c); col[u] == -1) col[u]= !c, ord[(c ? l++ : --r)]= que[t++]= u;
  int t= 0, k= 0;
  for (bool upd= true; upd; t= 0) {
   upd= false, rt.assign(n, -1), pre.assign(n, -1);
   for (int i= l; i--;)
    if (int v= ord[i]; mate[v] == -1) que[t++]= rt[v]= pre[v]= v;
   for (int i= 0; i < t; ++i)
    if (int v= que[i]; mate[rt[v]] == -1)
     for (int j= pos[v], u, w; j < pos[v + 1]; ++j) {
      if (w= mate[u= g[j]]; w == -1) {
       for (upd= true; u != -1; v= pre[v]) mate[u]= v, std::swap(mate[v], u);
       break;
      }
      if (pre[w] == -1) rt[que[t++]= w]= rt[pre[w]= v];
     }
  }
  for (int i= l; i--;)
   if (int s= ord[i]; rt[s] != -1)
    if (blg[s]= -1, s= mate[s]; s != -1) blg[s]= -1;
  for (int i= l; i < n; ++i)
   if (int s= ord[i]; mate[s] == -1) blg[que[t++]= s]= 0;
  for (int i= 0; i < t; ++i)
   for (int v= que[i], j= pos[v], u, w; j < pos[v + 1]; ++j)
    if (blg[u= g[j]] == -3)
     if (w= mate[u], blg[u]= 0; w != -1 && blg[w] == -3) blg[que[t++]= w]= 0;
  pre.assign(pos.begin(), pos.begin() + n), t= 0;
  for (int i= l; i--;)
   if (int v= ord[i]; blg[v] == -3)
    for (blg[v]= -4; v >= 0;) {
     if (pre[v] == pos[v + 1]) rt[t++]= v, v= blg[v];
     else if (int w= mate[g[pre[v]++]]; blg[w] == -3) blg[w]= v, v= w;
    }
  for (int i= 0, e= 0; t--;)
   if (int s= rt[t], p= mate[s]; blg[p] == -3)
    for (que[e++]= p, blg[s]= blg[p]= ++k; i < e; ++i)
     for (int v= que[i], j= pos[v]; j < pos[v + 1]; ++j)
      if (int u= g[j], w= mate[u]; blg[w] == -3) que[e++]= w, blg[u]= blg[w]= k;
  pos.assign(++k + 2, 0);
  for (int v= n; v--; ++pos[blg[v]])
   if (blg[v] == -1) blg[v]= k;
  for (int i= 0; i <= k; ++i) pos[i + 1]+= pos[i];
  for (int v= n; v--;) pre[--pos[blg[v]]]= v;
  std::vector<std::array<int, 2>> es_;
  for (auto &[u, v]: es) {
   if (col[u]) std::swap(u, v);
   if (int a= blg[u], b= blg[v]; a != b) es_.push_back({a, b});
  }
  std::sort(es_.begin(), es_.end()), es_.erase(std::unique(es_.begin(), es_.end()), es_.end());
  std::vector<int> dg0(es_.size()), dg1(es_.size()), sp0(k + 2), sp1(k + 2);
  for (auto [u, v]: es_) ++sp0[u], ++sp1[v];
  std::partial_sum(sp0.begin(), sp0.end(), sp0.begin()), std::partial_sum(sp1.begin(), sp1.end(), sp1.begin());
  for (auto [u, v]: es_) dg0[--sp0[u]]= v, dg1[--sp1[v]]= u;
  dag_[0]= {dg0, sp0}, dag_[1]= {dg1, sp1};
 }
 const std::vector<std::array<int, 2>> &edges() const { return es; }
 bool color(int v) const { return col[v]; }
 ConstListRange<int> left_vertices() const { return {ord.cbegin(), ord.cbegin() + l}; }
 ConstListRange<int> right_vertices() const { return {ord.cbegin() + l, ord.cend()}; }
 int match(int v) const { return mate[v]; }
 int component_num() const { return pos.size() - 1; }
 int belong(int v) const { return blg[v]; }
 ConstListRange<int> block(int k) const { return {pre.cbegin() + pos[k], pre.cbegin() + pos[k + 1]}; }
 template <bool rev> const CSRArray<int> &dag() { return dag_[rev]; }
 std::vector<std::array<int, 2>> max_matching() const {
  std::vector<std::array<int, 2>> ret;
  for (int i= l; i--;)
   if (int v= ord[i], u= mate[v]; u != -1) ret.push_back({v, u});
  return ret;
 }
 std::vector<int> min_vertex_cover() const {
  const int n= col.size(), m= component_num();
  std::vector<int> sel(m, -1), ret, que(m);
  sel[0]= 1, sel.back()= 0;
  for (int v= 0, k, s, c, i, t; v < n; ++v) {
   if (c= col[v], s= sel[k= blg[v]]; s == -1)
    for (sel[que[0]= k]= s= !c, i= 0, t= 1; i < t; ++i)
     for (int u: dag_[s][que[i]])
      if (sel[u] == -1) sel[u]= s, que[t++]= u;
   if (c ^ s) ret.push_back(v);
  }
  return ret;
 }
 std::vector<int> max_independent_set() const {
  const int n= col.size(), m= component_num();
  std::vector<int> sel(m, -1), ret, que(m);
  sel[0]= 0, sel.back()= 1;
  for (int v= 0, k, s, c, i, t; v < n; ++v) {
   if (c= col[v], s= sel[k= blg[v]]; s == -1)
    for (sel[que[0]= k]= s= !c, i= 0, t= 1; i < t; ++i)
     for (int u: dag_[c][que[i]])
      if (sel[u] == -1) sel[u]= s, que[t++]= u;
   if (c ^ s) ret.push_back(v);
  }
  return ret;
 }
};