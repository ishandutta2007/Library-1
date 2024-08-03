// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc327/tasks/abc327_d
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/BipartiteGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i].first;
 for (int i= 0; i < M; ++i) cin >> g[i].second;
 for (auto &e: g) --e;
 auto col= paint_two_colors(g);
 cout << (col.empty() ? "No" : "Yes") << '\n';
 return 0;
}
