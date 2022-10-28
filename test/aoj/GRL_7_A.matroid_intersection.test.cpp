#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/7/GRL_7_A"
// 二部マッチング (分割+分割)
#include <bits/stdc++.h>
#include "src/Optimization/matroid_intersection.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int L, R, M;
  cin >> L >> R >> M;
  vector<vector<int>> partl(L), partr(R);
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    partl[a].push_back(i), partr[b].push_back(i);
  }
  PartitionMatroid M1(M, partl, vector<int>(L, 1));
  PartitionMatroid M2(M, partr, vector<int>(R, 1));
  auto S = matroid_intersection(M, M1, M2);
  cout << S.size() << '\n';
  return 0;
}