#pragma once
#include <vector>
#include <tuple>
#include <array>
#include <cassert>
#include <type_traits>
template <typename T> class LinearSystemIncidence {
 const int n;
 std::vector<std::array<int, 2>> es;
public:
 LinearSystemIncidence(int n): n(n) {}
 void add_edge(int src, int dst) { es.push_back({src, dst}); }
 std::vector<T> solve(std::vector<T> b) const {
  assert((int)b.size() == n);
  const int m= es.size();
  std::vector<T> x(m);
  std::vector<int> adj(m + m), pos(n + 1, 0);
  for (auto [s, d]: es) ++pos[s], ++pos[d];
  for (int i= 0; i < n; ++i) pos[i + 1]+= pos[i];
  for (int i= m; i--;) adj[--pos[es[i][0]]]= i, adj[--pos[es[i][1]]]= i;
  std::vector<int> pre(n, -2), ei(pos.begin(), pos.begin() + n);
  for (int s= 0, p, e, q, f; s < n; ++s)
   if (pre[s] == -2)
    for (pre[p= s]= -1;;) {
     if (ei[p] == pos[p + 1]) {
      if (e= pre[p]; e < 0) {
       if (b[p] != T()) return {};  // no solution
       break;
      }
      T tmp= b[p];
      p= es[e][f= (es[e][0] == p)];
      if constexpr (std::is_same_v<T, bool>) x[e]= tmp, b[p]= tmp ^ b[p];
      else x[e]= f ? -tmp : tmp, b[p]+= tmp;
     } else if (e= adj[ei[p]++], q= es[e][es[e][0] == p]; pre[q] == -2) pre[p= q]= e;
    }
  return x;
 }
};