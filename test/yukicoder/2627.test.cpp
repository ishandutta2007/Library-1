// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2627
// competitive-verifier: TLE 2
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, K, L, U;
 cin >> N >> K >> L >> U;
 PiecewiseLinearConvex<long long> f;
 vector<long long> vl[K], vu[K];
 for (int i= 0; i < N; ++i) {
  long long A;
  cin >> A;
  long long xl= L > A ? (A - L + 1) / K - 1 : (A - L) / K;
  f.add_ramp(1, xl);
  vl[((A - L) % K + K + 1) % K].push_back(xl);
  long long xu= U < A ? (A - U - 1) / K + 1 : (A - U) / K;
  f.add_ramp(-1, xu);
  vu[((A - U) % K + K) % K].push_back(xu);
 }

 long long ans= f.min().value();
 for (int i= 1; i < K; ++i) {
  for (long long x: vl[i]) f.subtract_ramp(1, x), f.add_ramp(1, x - 1);
  for (long long x: vu[i]) f.subtract_ramp(-1, x), f.add_ramp(-1, x - 1);
  ans= min(ans, (long long)f.min().value());
 }
 cout << ans << '\n';
 return 0;
}
