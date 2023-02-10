#define PROBLEM "https://judge.yosupo.jp/problem/biconnected_components"
#include <iostream>
#include "src/Graph/BiConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;

 cin >> N >> M;
 BiConnectedComponents bcc(N);
 for (int i= 0; i < M; ++i) {
  int a, b;
  cin >> a >> b;
  bcc.add_edge(a, b);
 }
 auto bct= bcc.block_cut_tree();
 int K= bct.size();
 cout << K - N << '\n';
 for (int i= N; i < K; i++) {
  cout << bct[i].size();
  for (int v: bct[i]) cout << " " << v;
  cout << '\n';
 }
 return 0;
}
