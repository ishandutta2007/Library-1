// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/215
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/bostan_mori.hpp"
#include "src/FFT/convolve.hpp"
using namespace std;

using Mint= ModInt<int(1e9 + 7)>;
Mint dp1[301][300 * 13 + 1];
Mint dp2[301][300 * 12 + 1];
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N;
 int P, C;
 cin >> N >> P >> C;
 int max_p= 13 * P + 1, max_c= 12 * C + 1;
 dp1[0][0]= 1;
 dp2[0][0]= 1;
 for (int v: {2, 3, 5, 7, 11, 13})
  for (int i= 0; i < P; i++)
   for (int j= 0; j + v < max_p; j++) dp1[i + 1][j + v]+= dp1[i][j];
 for (int v: {4, 6, 8, 9, 10, 12})
  for (int i= 0; i < C; i++)
   for (int j= 0; j + v < max_c; j++) dp2[i + 1][j + v]+= dp2[i][j];
 vector<Mint> p(dp1[P], dp1[P] + max_p), c(dp2[C], dp2[C] + max_c);
 auto f= convolve<Mint, 1 << 19>(p, c), g= f;
 int d= f.size();
 for (int i= 0; i < d; i++) f[0]-= f[i];
 for (int i= 1; i < d; i++) f[i]+= f[i - 1];
 g[0]-= 1;
 cout << div_at<Mint, 1 << 19>(f, g, N - 1) << '\n';
 return 0;
}
