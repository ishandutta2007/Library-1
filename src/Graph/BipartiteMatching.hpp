#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
class BipartiteMatching {
 std::vector<std::array<int, 2>> es;
 std::vector<int> lmate, rmate, rt;
public:
 BipartiteMatching() {}
 BipartiteMatching(int L, int R): lmate(L, -1), rmate(R, -1) {}
 void add_edge(int l, int r) { es.push_back({l, r}); }
 void erase_edge(int l, int r) {
  auto it= std::find(es.begin(), es.end(), std::array{l, r});
  if (assert(it != es.end()), es.erase(it); lmate[l] == r) lmate[l]= rmate[r]= -1;
 }
 template <bool lex= false> void build() {
  const int n= lmate.size();
  std::vector<int> g(es.size()), pos(n + 1), pre, que(n);
  if constexpr (lex) std::sort(es.rbegin(), es.rend());
  for (auto [l, r]: es) ++pos[l];
  for (int i= 0; i < n; ++i) pos[i + 1]+= pos[i];
  for (auto [l, r]: es) g[--pos[l]]= r;
  for (bool upd= true; upd;) {
   upd= false, rt.assign(n, -1), pre.assign(n, -1);
   int t= 0;
   for (int l= n; l--;)
    if (lmate[l] == -1) que[t++]= rt[l]= pre[l]= l;
   for (int i= 0; i < t; ++i)
    if (int l= que[i]; lmate[rt[l]] == -1)
     for (int j= pos[l], r, nl; j < pos[l + 1]; ++j) {
      if (nl= rmate[r= g[j]]; nl == -1) {
       for (upd= true; r != -1; l= pre[l]) rmate[r]= l, std::swap(lmate[l], r);
       break;
      }
      if (pre[nl] == -1) rt[que[t++]= nl]= rt[pre[nl]= l];
     }
  }
  if constexpr (lex) {
   std::vector<char> alive(n, 1);
   for (int v= 0, l, r; v < n; ++v)
    if (int u= lmate[v]; u != -1) {
     for (pre.assign(n, lmate[v]= rmate[u]= -1), pre[l= v]= -2, que.assign(pos.begin(), pos.begin() + n);;) {
      if (que[l] == pos[l + 1]) l= pre[l];
      else if (r= g[que[l]++], u= rmate[r]; u == -1) {
       for (; r != -1; l= pre[l]) rmate[r]= l, std::swap(lmate[l], r);
       break;
      } else if (alive[u] && pre[u] == -1) pre[u]= l, l= u;
     }
     alive[v]= 0;
    }
  }
 }
 inline size_t left_size() const { return lmate.size(); }
 inline size_t right_size() const { return rmate.size(); }
 inline int l_to_r(int l) const { return lmate[l]; }
 inline int r_to_l(int r) const { return rmate[r]; }
 std::vector<std::array<int, 2>> edges() const { return es; }
 std::vector<std::array<int, 2>> max_matching() const {
  std::vector<std::array<int, 2>> ret;
  for (int l= 0, n= lmate.size(); l < n; ++l)
   if (int r= lmate[l]; r != -1) ret.push_back({l, r});
  return ret;
 }
 bool used_as_vertex_cover_left(int l) const { return rt[l] == -1; }
 bool used_as_vertex_cover_right(int r) const { return r= rmate[r], r != -1 && rt[r] != -1; }
};