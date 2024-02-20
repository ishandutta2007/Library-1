#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_B"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int V, E;
 cin >> V >> E;
 Graph g(V, E);
 for (int i= 0; i < E; ++i) cin >> g[i];
 IncrementalBridgeConnectivity ibc(V);
 for (auto [u, v]: g) ibc.add_edge(u, v);
 vector<Edge> ans;
 for (auto [u, v]: g)
  if (!ibc.two_edge_connected(u, v)) ans.emplace_back(minmax(u, v));
 sort(ans.begin(), ans.end());
 for (auto [a, b]: ans) cout << a << " " << b << '\n';
 return 0;
}