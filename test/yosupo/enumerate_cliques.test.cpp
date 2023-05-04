#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_cliques"
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Graph/CliqueProblem.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 vector<Mint> x(N);
 for (int i= 0; i < N; ++i) cin >> x[i];
 CliqueProblem graph(N);
 while (M--) {
  int u, v;
  cin >> u >> v;
  graph.add_edge(u, v);
 }
 Mint ans= 0;
 graph.enumerate_cliques([&](const auto& clique) {
  Mint prod= 1;
  for (auto i: clique) prod*= x[i];
  ans+= prod;
 });
 cout << ans << '\n';
 return 0;
}