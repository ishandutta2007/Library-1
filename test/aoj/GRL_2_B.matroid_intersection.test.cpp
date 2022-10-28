#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_2_B"
// 最小全域有向木(グラフ+分割)
#include <bits/stdc++.h>
#include "src/Optimization/matroid_intersection.hpp"
using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N, M, r;
  cin >> N >> M >> r;
  GraphicMatroid M1(N);
  vector<int> w(M);
  vector<vector<int>> parts(N);
  for (int i = 0; i < M; i++) {
    int s, t;
    cin >> s >> t >> w[i];
    M1.add_edge(s, t);
    parts[t].push_back(i);
  }
  PartitionMatroid M2(M, parts, vector<int>(N, 1));
  auto S = weighted_matroid_intersection<-1>(M, M1, M2, w);
  int ans = 0;
  for (int e : S[N - 1]) ans += w[e];
  cout << ans << '\n';
  return 0;
}