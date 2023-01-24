#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_B"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int V, E;
 cin >> V >> E;
 IncrementalBridgeConnectivity ibc(V);
 int s[E], t[E];
 for (int i= 0; i < E; ++i) {
  cin >> s[i] >> t[i];
  ibc.add_edge(s[i], t[i]);
 }
 vector<array<int, 2>> ans;
 for (int i= 0; i < E; ++i)
  if (ibc.connected(s[i], t[i]) && !ibc.two_edge_connected(s[i], t[i])) {
   auto [a, b]= minmax(s[i], t[i]);
   ans.push_back({a, b});
  }
 sort(ans.begin(), ans.end());
 for (auto [a, b]: ans) cout << a << " " << b << '\n';
 return 0;
}