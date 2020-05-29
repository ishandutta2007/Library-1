#define PROBLEM "https://yukicoder.me/problems/642"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/MatchingBipartite.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  MatchingBipartite graph(N, N);
  for (int i = 0; i < N; i++) {
    int A;
    cin >> A;
    for (int j = 0; j < N; j++)
      if (j != A) graph.add_edge(i, j);
  }
  auto ans = graph.get_matching();
  if (ans.first != N) {
    cout << -1 << endl;
  } else {
    for (int a : ans.second.first) cout << a << endl;
  }
  return 0;
}