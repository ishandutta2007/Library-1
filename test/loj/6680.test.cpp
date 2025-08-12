// competitive-verifier: PROBLEM https://loj.ac/p/6680
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int n;
 cin >> n;
 auto a= [](int p, short e) {
  Mint ret= 1;
  for (; e; --e) ret+= (p ^ e);
  return ret;
 };
 auto b= [](int p, short e) {
  Mint ret= 0, pw= 1;
  for (; e; --e) ret+= pw * (p ^ e), pw*= p;
  return ret+= pw, ret;
 };
 auto A= multiplicative_table<Mint>(n, a);
 auto B= multiplicative_table<Mint>(n, b);
 vector<Mint> f(n + 1);
 for (int i= 1; i <= n; ++i) f[i]= B[i] * 2 - A[i];
 for (int i= 1; i <= n; ++i) f[i]+= f[i - 1];
 unsigned ans= 0;
 for (int i= 1; i <= n; ++i) ans^= f[i].val();
 cout << ans << '\n';
 return 0;
}