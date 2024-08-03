// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2264
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N;
 Mint M;
 cin >> N >> M;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 int l= 1;
 for (int a: A) l= lcm(l, a);
 ArrayOnDivisors<int, int> X(l);
 X.set_totient();
 Mint ans= 0;
 for (auto [d, phi]: X) {
  long long e= 0, k= l / d;
  for (int a: A) e+= gcd(a, k);
  ans+= M.pow(e) * phi;
 }
 ans/= l;
 cout << ans << '\n';
 return 0;
}