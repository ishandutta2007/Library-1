// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1501
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/Rational.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Q= Rational<long long>;
 int N, K;
 cin >> N >> K;
 int A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 if (N == 1) return cout << "1/1" << '\n', 0;
 Q d= 0, n= 0;
 for (int i= 0; i < N - 1; ++i) {
  d+= Q(1, A[i] * A[i + 1]);
  if (i == K - 2) n= d;
 }
 Q ans= n / d;
 if (ans == 0) cout << 0 << '\n';
 else cout << ans.num << "/" << ans.den << '\n';
 return 0;
}