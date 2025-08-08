// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc223/tasks/abc223_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/BipartiteGraph.hpp"
#include "src/Graph/DulmageMendelsohn.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 auto [bg, nw, ori]= graph_to_bipartite(g);
 int L= bg.left_size();
 DulmageMendelsohn dm(bg);
 int k= dm.size();
 int ans= 0;
 for (int v: dm.block(0))
  if (v >= L) ++ans;
 for (int v: dm.block(k - 1))
  if (v < L) ++ans;
 cout << ans << '\n';
 return 0;
}