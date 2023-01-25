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
 auto ans= scc.get_block();
 cout << ans.size() << '\n';
 for (const auto &blk: ans) {
  cout << blk.size();
  for (int v: blk) cout << " " << v;
  cout << '\n';
 }
 return 0;
}