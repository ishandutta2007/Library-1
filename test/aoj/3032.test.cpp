#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3032"
#include <bits/stdc++.h>
#include "src/Optimization/Matching.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, A, B;
  cin >> N >> A >> B;
  Matching graph(2 * N);
  int a[N], b[N];
  for (int i = 0; i < N; i++) cin >> a[i] >> b[i];
  for (int i = 0; i < N; i++) {
    if (abs(a[i] - b[i]) <= A ||
        (B <= abs(a[i] - b[i]) && abs(a[i] - b[i]) <= 2 * A))
      graph.add_edge(i, i + N);
    for (int j = i + 1; j < N; j++) {
      int tmp = abs(a[i] + a[j] - b[i] - b[j]);
      if (tmp <= A || (B <= tmp && tmp <= 2 * A)) graph.add_edge(i, j);
    }
  }
  cout << graph.get_matching().first << '\n';
  return 0;
}