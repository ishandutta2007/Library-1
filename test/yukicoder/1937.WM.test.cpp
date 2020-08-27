#define PROBLEM "https://yukicoder.me/problems/1937"

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
  int A[N];
  for (int i = 0; i < N; i++) cin >> A[i];
  int L[N], R[N];
  vector<int> vec(A, A + N);
  for (int i = 0; i < N; i++) {
    cin >> L[i] >> R[i];
    vec.push_back(A[i] - L[i]);
    vec.push_back(A[i] + R[i]);
  }
  sort(vec.begin(), vec.end());
  vec.erase(unique(vec.begin(), vec.end()), vec.end());
  auto id = [&](int x) {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin();
  };
  vector<int> l(N), r(N);
  for (int i = 0; i < N; i++) {
    l[i] = id(A[i] - L[i]);
    r[i] = id(A[i] + R[i]);
  }
  WaveletMatrix wml(l), wmr(r);
  long long ans = 0;
  for (int i = 0; i < N; i++) {
    int left = lower_bound(A, A + N, A[i] - L[i]) - A;
    int right = upper_bound(A, A + N, A[i] + R[i]) - A;
    int ai = id(A[i]);
    ans += wmr.range_freq(left, i, ai, 4 * N);
    ans += wml.range_freq(i + 1, right, 0, ai + 1);
  }
  cout << ans / 2 << "\n";
  return 0;
}