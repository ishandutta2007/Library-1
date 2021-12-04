#define PROBLEM "https://atcoder.jp/contests/arc106/tasks/arc106_b"

#include <bits/stdc++.h>
#include "src/Graph/LinearSystemIncidence.hpp"
using namespace std;

// 解無しの判定のverify

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N, M;
  cin >> N >> M;
  LinearSystemIncidence<long long> graph(N);
  vector<long long> a(N), b(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  for (int i = 0; i < N; i++) cin >> b[i], b[i] -= a[i];
  for (int i = 0; i < M; i++) {
    int c, d;
    cin >> c >> d;
    graph.add_edge(--c, --d);
  }
  if (M)
    cout << (graph.solve(b).empty() ? "No" : "Yes") << '\n';
  else
    cout << (b[0] ? "No" : "Yes") << '\n';
  return 0;
}