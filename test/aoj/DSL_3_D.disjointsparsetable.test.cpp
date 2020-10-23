#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/3/DSL_3_D"
#include <bits/stdc++.h>
#include "src/DataStructure/DisjointSparseTable.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, L;
  cin >> N >> L;
  vector<int> a(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  auto f = [](int a, int b) { return min(a, b); };
  DisjointSparseTable<int> dst(a, f);
  for (int i = 0; i + L <= N; i++) {
    if (i) cout << " ";
    cout << dst.query(i, i + L);
  }
  cout << endl;
  return 0;
}