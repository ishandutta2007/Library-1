// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2304
// competitive-verifier: TLE 1
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
 f.add_inf();
 for (int i= 0; i < N; ++i) {
  f.add_inf(true);
  f.add_linear(1);
  f.add_linear(-A[i]);
  f.chmin_slide_win(-1, 1);
  f.add_linear(A[i]);
 }
 cout << (long long)-f(0).value() << '\n';
 return 0;
}