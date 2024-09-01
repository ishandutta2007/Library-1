// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/assignment
// competitive-verifier: TLE 2
// competitive-verifier: MLE 128
// 　二部グラフ
#include <iostream>
#include <vector>
#include "src/Optimization/WeightedMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 WeightedMatching<long long, true> graph(N + N);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) {
   long long a;
   cin >> a;
   graph.add_edge(i, N + j, a);
  }
 graph.build();
 long long ans= 0;
 for (auto [u, v, w]: graph.weight_matching()) ans+= w;
 cout << ans << '\n';
 for (int i= 0; i < N; ++i) cout << graph.match(i) - N << " \n"[i == N - 1];
 return 0;
}