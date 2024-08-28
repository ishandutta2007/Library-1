// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0334
// competitive-verifier: TLE 0.5
// 辞書順マッチングのverify
#include <iostream>
#include "src/Graph/BipartiteGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N, --N;
 BipartiteGraph bg(N, N);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) {
   int x;
   cin >> x;
   if (x) bg.add_edge(j, i + N);
  }
 auto [mc, mate]= bipartite_matching<true>(bg);
 if (mc.size() < N) cout << "no" << '\n';
 else {
  cout << "yes" << '\n';
  for (int i= 0; i < N; ++i) cout << mate[i] - N + 1 << '\n';
 }
 return 0;
}