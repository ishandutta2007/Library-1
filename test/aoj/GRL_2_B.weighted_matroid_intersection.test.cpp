// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/2/GRL_2_B
// 重み付き
// 最小全域有向木(グラフ+分割)
#include <iostream>
#include <vector>
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
 for (int i= 0; i < M; i++) {
  int s, t;
  cin >> s >> t >> w[i];
  M1.add_edge(s, t);
  parts[t].push_back(i);
 }
 vector<int> R(N, 1);
 R[r]= 0;
 PartitionMatroid M2(M, parts, R);
 auto S= weighted_matroid_intersection<MINIMIZE>(M, M1, M2, w);
 int ans= 0;
 for (int e: S[N - 1]) ans+= w[e];
 cout << ans << '\n';
 return 0;
}