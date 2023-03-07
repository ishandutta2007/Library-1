#define PROBLEM "https://judge.yosupo.jp/problem/scc"
#include <iostream>
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 StronglyConnectedComponents scc(N);
 for (int i= 0; i < M; ++i) {
  int a, b;
  cin >> a >> b;
  scc.add_edge(a, b);
 }
 scc.build();
 int C= scc.components_num();
 cout << C << '\n';
 for (int i= 0; i < C; ++i) {
  auto blk= scc.block(i);
  cout << blk.size();
  for (int v: blk) cout << " " << v;
  cout << '\n';
 }
 return 0;
}