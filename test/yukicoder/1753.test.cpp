// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1753
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<Mint> A(N + 1);
 Mint sum= 0;
 for (int i= 0; i <= N; ++i) cin >> A[i], sum+= A[i];
 Mint iv= Mint(1) / sum;
 for (int i= 0; i <= N; ++i) A[i]*= iv;
 Mint ans= 0;
 vector<Mint> b(1024);
 b[0]= A[0], sps::hadamard(b);
 for (int X= 1; X < 1024; ++X) {
  vector<Mint> a(1024);
  for (int i= 1; i <= N; ++i)
   if (i != X) a[i]= A[i];
  sps::hadamard(a);
  for (int i= 0; i < 1024; ++i) a[i]= b[i] / (Mint(1) - a[i]);
  sps::hadamard(a);
  ans+= a[X];
 }
 ans/= 1024;
 cout << ans << '\n';
 return 0;
}