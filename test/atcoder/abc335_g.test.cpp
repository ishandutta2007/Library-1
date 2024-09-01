// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc335/tasks/abc335_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/NumberTheory/OrderFp.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N, P;
 cin >> N >> P;
 vector<long long> a(N);
 OrderFp ord(P);
 for (int i= 0; i < N; i++) {
  long long A;
  cin >> A;
  a[i]= ord(A);
 }
 ArrayOnDivisors<long long, long long> x(P - 1, ord.factors);
 for (int i= 0; i < N; ++i) ++x[a[i]];
 x.divisor_zeta();
 long long ans= 0;
 for (int i= 0; i < N; ++i) ans+= x[a[i]];
 cout << ans << '\n';
 return 0;
}