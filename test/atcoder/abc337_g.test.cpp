#define PROBLEM "https://atcoder.jp/contests/abc337/tasks/abc337_g"
#include <iostream>
#include <vector>
#include "src/DataStructure/WaveletMatrix.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (auto &e: g) cin >> e, --e;
 HeavyLightDecomposition hld(g);
 vector<int> a(N);
 for (int v= N; v--;) a[hld.to_seq(v)]= v;
 WaveletMatrix wm(a);
 Graph g2(N);
 vector<int> val;
 for (auto [u, v]: g) {
  if (u != hld.parent(v)) swap(u, v);
  auto [l, r]= hld.subtree(v);
  g2.add_edge(u, v);
  val.push_back(wm.count(l, r, v));
  g2.add_edge(v, u);
  val.push_back(u - wm.count(l, r, u));
 }
 auto put_edge= [&](int, int e, long long d) { return d + val[e]; };
 auto op= [&](long long a, long long b) { return a + b; };
 auto put_vertex= [&](int, long long d) { return d; };
 Rerooting<long long> dp(g2, g2.adjacency_edge(1), hld, put_edge, op, 0ll, put_vertex);
 for (int i= 0; i < N; ++i) cout << dp[i] + i << " \n"[i == N - 1];
 return 0;
}
