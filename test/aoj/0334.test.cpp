#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0334"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Graph/MatchingBipartite.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  N--;
  MatchingBipartite graph(N, N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      int b;
      cin >> b;
      if (b) graph.add_edge(j, i);
    }
  auto ans = graph.lexicographically_matching();
  if (ans.first < N)
    cout << "no" << endl;
  else {
    cout << "yes" << endl;
    for (int i = 0; i < N; i++) cout << ans.second.first[i] + 1 << endl;
  }
  return 0;
}