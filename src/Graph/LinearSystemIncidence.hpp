#pragma once
#include <vector>
#include <tuple>
#include <type_traits>
template <typename T> class LinearSystemIncidence {
 int m;
 std::vector<std::vector<std::tuple<int, int, bool>>> adj;
public:
 LinearSystemIncidence(int n): m(0), adj(n) {}
 void add_edge(int src, int dst) { adj[src].emplace_back(m, dst, true), adj[dst].emplace_back(m++, src, false); }
 std::vector<T> solve(const std::vector<T> &b) const {
  std::vector<T> x(m);
  std::vector<bool> used(adj.size());
  auto dfs= [&](auto self, int u) -> T {
   used[u]= true;
   T ret= b[u];
   for (auto [id, to, fwd]: adj[u])
    if (!used[to]) {
     T tmp= self(self, to);
     if constexpr (std::is_same_v<T, bool>) x[id]= tmp, ret^= tmp;
     else x[id]= fwd ? tmp : -tmp, ret+= tmp;
    }
   return ret;
  };
  for (std::size_t u= adj.size(); u--;)
   if (!used[u] && dfs(dfs, u) != T(0)) return std::vector<T>();  // no sloutions
  return x;
 }
};