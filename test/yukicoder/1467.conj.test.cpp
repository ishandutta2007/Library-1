// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1467
// competitive-verifier: TLE 2
// competitive-verifier: MLE 128
#include <iostream>
#include "src/Misc/compress.hpp"
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int M, N;
 cin >> M >> N;
 long long A[M], B[N];
 for (int i= 0; i < M; ++i) cin >> A[i];
 for (int j= 0; j < N; ++j) cin >> B[j];
 vector<long long> vec(A, A + M);
 for (int j= 0; j < N; ++j) vec.push_back(B[j]);
 auto id= compress(vec);
 int n= vec.size();
 vector<long long> a(n), b(n);
 for (int i= 0; i < M; ++i) ++a[id(A[i])];
 for (int j= 0; j < N; ++j) ++b[id(B[j])];
 using PLC= PiecewiseLinearConvex<long long>;
 for (int k= 1; k <= M; ++k) {
  PLC f;
  for (int i= 0; i < n; ++i) {
   f.add_inf(true);
   f.add_linear(a[i] - b[i] * k);
   if (i < n - 1) {
    long long c= vec[i + 1] - vec[i];
    f.chmin_slide_win(-c, c);
   }
  }
  cout << (long long)-f.min().value() << '\n';
  PLC::reset();
 }
 return 0;
}