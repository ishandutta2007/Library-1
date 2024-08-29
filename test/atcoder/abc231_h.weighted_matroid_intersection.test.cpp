// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc231/tasks/abc231_h
// competitive-verifier: TLE 0.5
// 重み付き
// (分割 + 分割)
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Optimization/matroid_intersection.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int H, W, N;
 cin >> H >> W >> N;
 vector<long long> C(N);
 vector<vector<int>> parts1(H), parts2(W);
 vector<int> sz1(H, -1), sz2(W, -1);
 for (int i= 0; i < N; i++) {
  int A, B;
  cin >> A >> B >> C[i], A--, B--;
  parts1[A].push_back(i);
  parts2[B].push_back(i);
  sz1[A]++, sz2[B]++;
 }
 PartitionMatroid M1(N, parts1, sz1), M2(N, parts2, sz2);
 auto S= weighted_matroid_intersection<MAXIMIZE>(N, M1, M2, C);
 long long s= 0;
 for (int i= 1, ed= S.size(); i < ed; i++) {
  long long sum= 0;
  for (int e: S[i]) sum+= C[e];
  if (s < sum) s= sum;
 }
 cout << accumulate(C.begin(), C.end(), 0ll) - s << '\n';
 return 0;
}