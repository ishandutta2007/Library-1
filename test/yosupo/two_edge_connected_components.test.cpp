#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"
#include <iostream>
#include <vector>
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 IncrementalBridgeConnectivity ibc(N);
 for (int i= 0, a, b; i < M; i++) cin >> a >> b, ibc.add_edge(a, b);
 int n= 0;
 vector<int> ans[N];
 for (int i= 0; i < N; ++i) {
  int j= ibc.leader(i);
  ans[j].push_back(i);
  if (ans[j].size() == 1) ++n;
 }
 cout << n << '\n';
 for (int i= 0; i < N; ++i)
  if (!ans[i].empty()) {
   cout << ans[i].size();
   for (int v: ans[i]) cout << " " << v;
   cout << '\n';
  }
 return 0;
}