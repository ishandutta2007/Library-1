// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification
#include <iostream>
#include "src/Misc/compress.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
#include "src/Misc/rng.hpp"
#include "src/Misc/Pointwise.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N);
 for (int i= 1, p; i < N; ++i) cin >> p, g.add_edge(p, i);
 auto adj= g.adjacency_vertex(1);
 using Mint= ModInt<(1ll << 61) - 1>;
 using K= Pointwise<Mint, Mint>;
 vector<int> dep(N);
 vector<K> dp(N), hash(N);
 for (auto& x: hash) x= {rng(2, Mint::mod() - 1), rng(2, Mint::mod() - 1)};
 auto dfs= [&](auto&& dfs, int v) -> void {
  dp[v]= 1;
  for (int u: adj[v]) {
   dfs(dfs, u);
   dep[v]= max(dep[v], dep[u]);
   dp[v]*= dp[u] + hash[dep[u]];
  }
  ++dep[v];
 };
 dfs(dfs, 0);
 auto vec= dp;
 auto id= compress(vec);
 cout << vec.size() << '\n';
 for (int i= 0; i < N; ++i) cout << id(dp[i]) << " \n"[i == N - 1];
 return 0;
}