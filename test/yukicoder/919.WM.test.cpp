#define PROBLEM "https://yukicoder.me/problems/no/919"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/WaveletMatrix.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  vector<int> A(N);
  for (int i = 0; i < N; i++) cin >> A[i];
  vector<int> vec(A);
  sort(vec.begin(), vec.end());
  vec.erase(unique(vec.begin(), vec.end()), vec.end());
  auto id = [&](int x) {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin();
  };
  for (int i = 0; i < N; i++) A[i] = id(A[i]);
  WaveletMatrix wm(A);
  long long ans = 0;
  for (int K = 1; K <= N; K++) {
    int M = N / K, med = (K - 1) / 2;
    long long L[M + 1], R[M + 1], Lsum = 0, Rsum = 0;
    L[0] = R[0] = 0;
    for (int i = 1; i <= M; i++) {
      Lsum += vec[wm.kth_smallest(K * (i - 1), K * i, med)];
      Rsum += vec[wm.kth_smallest(N - K * i, N - K * (i - 1), med)];
      L[i] = max(L[i - 1], Lsum);
      R[i] = max(R[i - 1], Rsum);
    }
    for (int i = 0; i <= M; i++) ans = max(ans, (L[i] + R[M - i]) * K);
  }
  cout << ans << "\n";
  return 0;
}