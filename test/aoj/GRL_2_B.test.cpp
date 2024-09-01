// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/2/GRL_2_B
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/minimum_spanning_aborescence.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, r;
 cin >> N >> M >> r;
 Graph g(N, M);
 vector<int> w(M);
 for (int i= 0; i < M; ++i) cin >> g[i] >> w[i];
 auto [ans, _]= minimum_spanning_aborescence(g, w, r);
 cout << ans << '\n';
 return 0;
}