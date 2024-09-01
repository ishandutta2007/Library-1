// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2304
// competitive-verifier: TLE 1
// competitive-verifier: MLE 128
#include <iostream>
#include <algorithm>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 sort(A, A + N);
 PiecewiseLinearConvex<long long> f;
 for (int i= 0; i < N; ++i) {
  f.chmin_cum();
  f.shift(1);
  f.add_abs(1, A[i]);
 }
 cout << (long long)f.min().value() << '\n';
 return 0;
}