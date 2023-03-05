#pragma once
#include <vector>
#include <tuple>
#include <array>
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
  for (int i= 1; i <= n; ++i) pos[i]+= pos[i - 1];
  for (int i= m; i--;) {
   auto [s, d]= es[i];
   adj[--pos[s]]= i, adj[--pos[d]]= i;
  }
  std::vector<int> pre(n, -2), dat(pos.begin(), pos.begin() + n);
  for (int s= 0, p, e, q, f; s < n; ++s)
   if (pre[s] == -2)
    for (pre[p= s]= -1;;) {
     if (dat[p] == pos[p + 1]) {
      if (e= pre[p]; e < 0) {
       if (b[p] != T()) return {};  // no solution
       break;
      }
      T tmp= b[p];
      p= es[e][f= (es[e][0] == p)];
      if constexpr (std::is_same_v<T, bool>) x[e]= tmp, b[p]= tmp ^ b[p];
      else x[e]= f ? -tmp : tmp, b[p]+= tmp;
     } else if (e= adj[dat[p]++], q= es[e][es[e][0] == p]; pre[q] == -2) pre[p= q]= e;
    }
  return x;
 }
};