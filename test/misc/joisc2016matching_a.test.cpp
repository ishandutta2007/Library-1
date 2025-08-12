// competitive-verifier: PROBLEM https://atcoder.jp/contests/joisc2016matching/tasks/joisc2016matching_a
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/general_matching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  int N, M;
  cin >> N >> M;
  Graph g(N, M);
  for (int i= 0; i < M; ++i) cin >> g[i];
  auto [_, ans]= general_matching(g);
  for (int i= 0; i < N; ++i) cout << ans[i] << " \n"[i == N - 1];
 }
 return 0;
}