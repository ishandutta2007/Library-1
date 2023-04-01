#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/1/DPL_1_H"
#include <iostream>
#include <vector>
#include <algorithm>
#include <src/Optimization/Knapsack.hpp>
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 Knapsack<int64_t, int64_t> K;
 int N;
 int64_t W;
 cin >> N >> W;
 for (int i= 0; i < N; ++i) {
  int64_t v, w;
  cin >> v >> w;
  K.add(v, w);
 }
 K.build();
 cout << K.solve(W) << '\n';
 return 0;
}