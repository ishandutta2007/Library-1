// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/215
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/bostan_mori.hpp"
#include "src/FFT/Polynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 using Poly= Polynomial<Mint, 1 << 19>;
 auto x= Poly::x();
 long long N;
 int P, C;
 cin >> N >> P >> C;
 vector<Poly> a(P + 1), b(C + 1);
 a[0]= {1}, b[0]= {1};
 for (int v: {2, 3, 5, 7, 11, 13})
  for (int i= 1; i <= P; i++) a[i]+= a[i - 1] * (x ^ v);
 for (int v: {4, 6, 8, 9, 10, 12})
  for (int i= 1; i <= C; i++) b[i]+= b[i - 1] * (x ^ v);
 auto f= a[P] * b[C];
 cout << div_at<Mint, 1 << 19>((f(1) - f) / (x - 1), f - 1, N - 1) << '\n';
 return 0;
}