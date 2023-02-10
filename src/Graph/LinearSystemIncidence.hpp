#pragma once
#include <vector>
#include <tuple>
#include <array>
#include <type_traits>
template <typename T> class LinearSystemIncidence {
 std::vector<std::array<int, 2>> es;
 std::vector<std::vector<int>> adj;
public:
 LinearSystemIncidence(int n): adj(n) {}
 void add_edge(int src, int dst) {
  int m= es.size();
  adj[src].push_back(m), adj[dst].push_back(m), es.push_back({src, dst});
 }
 std::vector<T> solve(std::vector<T> b) const {
  const int n= adj.size();
  std::vector<T> x(es.size());
  std::vector<int> pre(n, -2), dat(n, 0);
  for (int s= 0, p, e, q, f; s < n; ++s)
   if (pre[s] == -2)
    for (pre[p= s]= -1;;) {
     if (dat[p] == (int)adj[p].size()) {
      if (e= pre[p]; e < 0) {
       if (b[p] != T()) return {};  // no solution
       break;
      }
      f= (es[e][0] == p), q= es[e][f];
      T tmp= b[p];
      if constexpr (std::is_same_v<T, bool>) x[e]= tmp, b[q]= tmp ^ b[q];
      else x[e]= f ? -tmp : tmp, b[q]+= tmp;
      p= q;
      continue;
     }
     if (e= adj[p][dat[p]++], q= es[e][es[e][0] == p]; pre[q] == -2) pre[q]= e, p= q;
    }
  return x;
 }
};