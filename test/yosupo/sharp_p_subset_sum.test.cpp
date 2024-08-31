// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sharp_p_subset_sum
// competitive-verifier: TLE 1
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_exp.hpp"
using namespace std;
// log(1+x^s_1)(1+x^s_2)...(1+x^s_N)=log(1+x^s_1)+log(1+x^s_2)+...log(1+x^s_N)
// log(1+x)=x-x^2/2+x^3/3-x^4/4...
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, T;
 cin >> N >> T;
 int c[T + 1];
 fill_n(c, T + 1, 0);
 for (int i= 0; i < N; i++) {
  int s;
  cin >> s, c[s]++;
 }
 vector<Mint> a(T + 1);
 for (int t= 1; t <= T; t++)
  if (c[t])
   for (int j= 1; j * t <= T; j++) {
    Mint tmp= Mint(c[t]) / j;
    a[j * t]+= j & 1 ? tmp : -tmp;
   }
 auto ans= exp(a);
 for (int t= 1; t <= T; t++) cout << ans[t] << " \n"[t == T];
 return 0;
}