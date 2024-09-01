// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/scc
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i];
 StronglyConnectedComponents scc(g);
 int C= scc.size();
 cout << C << '\n';
 for (int i= 0; i < C; ++i) {
  auto blk= scc.block(i);
  cout << blk.size();
  for (int v: blk) cout << " " << v;
  cout << '\n';
 }
 return 0;
}