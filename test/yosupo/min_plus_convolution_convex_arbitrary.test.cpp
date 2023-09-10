#define PROBLEM "https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary"
#include <iostream>
#include "src/Optimization/monotone_minima.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 long long a[N], b[M];
 for (int i= 0; i < N; ++i) cin >> a[i];
 for (int j= 0; j < M; ++j) cin >> b[j];
 auto select= [&](int i, int j, int k) {
  if (i < k) return false;
  if (i - j >= N) return true;
  return a[i - j] + b[j] >= a[i - k] + b[k];
 };
 auto r= monotone_minima(N + M - 1, M, select);
 for (int i= 0; i < N + M - 1; ++i) cout << a[i - r[i]] + b[r[i]] << " \n"[i == N + M - 2];
 return 0;
}