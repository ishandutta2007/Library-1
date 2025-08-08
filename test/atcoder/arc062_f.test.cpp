// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc062/tasks/arc062_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc062/tasks/arc062_d
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Math/ModInt.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Graph/block_cut_tree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 using F= FactorialPrecalculation<Mint>;
 int N, M, K;
 cin >> N >> M >> K;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i];
 auto bct= block_cut_tree(g).adjacency_vertex(0);
 int n= bct.size();
 vector<int> es[n - N];
 for (int e= M; e--;) {
  auto [s, d]= g[e];
  int x;
  for (int u: bct[s])
   for (int v: bct[d])
    if (u == v) x= u;
  es[x - N].push_back(e);
 }
 Mint ans= 1;
 for (int i= N; i < n; ++i) {
  int m= bct[i].size();
  if (m == 2) {
   ans*= K;
   continue;
  }
  vector<int> deg(N);
  for (int e: es[i - N]) {
   auto [a, b]= g[e];
   ++deg[a], ++deg[b];
  }
  bool is_cyc= true;
  for (int u: bct[i]) is_cyc&= (deg[u] == 2);
  if (is_cyc) {
   Mint cnt= 0;
   for (int j= 1; j <= m; ++j) cnt+= Mint(K).pow(gcd(m, j));
   cnt/= m;
   ans*= cnt;
  } else ans*= F::nHr(K, es[i - N].size());
 }
 cout << ans << '\n';
 return 0;
}
