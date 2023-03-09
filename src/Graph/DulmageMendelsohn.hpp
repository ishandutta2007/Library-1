#pragma once
#include <array>
#include <algorithm>
#include <numeric>
#include "src/DataStructure/CsrArray.hpp"
class DulmageMendelsohn {
 std::vector<int> blg[2], mate[2], pos[2];
 std::vector<std::array<int, 2>> es;
public:
 DulmageMendelsohn(int L, int R): blg{std::vector(L, -3), std::vector(R, -3)} {}
 void add_edge(int l, int r) { es.push_back({l, r}); }
 void build() {
  const int n[]= {(int)blg[0].size(), (int)blg[1].size()};
  std::vector<int> g[2], rt, pre, que(std::max(n[0], n[1]));
  for (int b= 2; b--;) g[b].resize(es.size()), pos[b].resize(n[b] + 1), mate[b].assign(n[b], -1);
  for (auto [l, r]: es) ++pos[0][l], ++pos[1][r];
  for (int b= 2; b--;) std::partial_sum(pos[b].begin(), pos[b].end(), pos[b].begin());
  for (auto [l, r]: es) g[0][--pos[0][l]]= r, g[1][--pos[1][r]]= l;
  int t= 0, k= 0;
  for (bool upd= true; upd; t= 0) {
   upd= false, rt.assign(n[0], -1), pre.assign(n[0], -1);
   for (int l= n[0]; l--;)
    if (mate[0][l] == -1) que[t++]= rt[l]= pre[l]= l;
   for (int i= 0; i < t; ++i)
    if (int l= que[i]; mate[0][rt[l]] == -1)
     for (int j= pos[0][l], r, nl; j < pos[0][l + 1]; ++j) {
      if (nl= mate[1][r= g[0][j]]; nl == -1) {
       for (upd= true; r != -1; l= pre[l]) mate[1][r]= l, std::swap(mate[0][l], r);
       break;
      }
      if (pre[nl] == -1) rt[que[t++]= nl]= rt[pre[nl]= l];
     }
  }
  for (int s= n[0], v; s--;)
   if (rt[s] != -1)
    if (blg[0][s]= -1, v= mate[0][s]; v != -1) blg[1][v]= -1;
  for (int s= n[1]; s--;)
   if (mate[1][s] == -1) blg[1][que[t++]= s]= 0;
  for (int i= 0; i < t; ++i)
   for (int v= que[i], j= pos[1][v], u, w; j < pos[1][v + 1]; ++j)
    if (blg[0][u= g[1][j]] == -3)
     if (w= mate[0][u], blg[0][u]= 0; w != -1 && blg[1][w] == -3) blg[1][que[t++]= w]= 0;
  pre.assign(pos[0].begin(), pos[0].begin() + n[0]), t= 0;
  for (int s= n[0], v; s--;)
   if (blg[0][s] == -3)
    for (blg[0][v= s]= -4; v >= 0;) {
     if (pre[v] == pos[0][v + 1]) rt[t++]= v, v= blg[0][v];
     else if (int w= mate[1][g[0][pre[v]++]]; blg[0][w] == -3) blg[0][w]= v, v= w;
    }
  for (int i= 0, e= 0; t--;)
   if (int s= rt[t], p= mate[0][s]; blg[1][p] == -3)
    for (que[e++]= p, blg[0][s]= blg[1][p]= ++k; i < e; ++i)
     for (int v= que[i], j= pos[1][v]; j < pos[1][v + 1]; ++j)
      if (int u= g[1][j], w= mate[0][u]; blg[1][w] == -3) que[e++]= w, blg[0][u]= blg[1][w]= k;
  ++k;
  for (int b= 2, s; b--;) {
   for (pos[b].assign(k + 1, 0), s= n[b]; s--; ++pos[b][blg[b][s]])
    if (blg[b][s] == -1) blg[b][s]= k;
   for (std::partial_sum(pos[b].begin(), pos[b].end(), pos[b].begin()), s= n[b]; s--;) mate[b][--pos[b][blg[b][s]]]= s;
  }
 }
 int component_num() const { return pos[0].size() - 1; }
 int left_belong(int l) const { return blg[0][l]; }
 int right_belong(int r) const { return blg[1][r]; }
 const ListRange<int> left_block(int k) const { return {mate[0].begin() + pos[0][k], mate[0].begin() + pos[0][k + 1]}; }
 const ListRange<int> right_block(int k) const { return {mate[1].begin() + pos[1][k], mate[1].begin() + pos[1][k + 1]}; }
};