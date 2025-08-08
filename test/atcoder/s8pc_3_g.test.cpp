// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/s8pc_3/tasks/s8pc_3_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/s8pc-3/tasks/s8pc_3_g
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/sample_points_shift.hpp"
#include "src/Math/bostan_mori.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 long long n, m;
 cin >> n >> m, --n, --m;
 auto f= sfps::div<Mint>({1}, {1, -1, -1}, n + 1);
 auto b= sfps::div<Mint>(sfps::pow<Mint>({1, -1}, n, n), {1, -1, -1}, n + 1);
 Mint D= Mint(1) / (b[n] * b[n] - b[n - 1] * b[n + 1]);
 Mint A= b[n] * f[n] - b[n - 1] * f[n + 1];
 Mint B= -b[n + 1] * f[n] + b[n] * f[n + 1];
 A*= D, B*= D;
 auto c= sfps::div<Mint>({A, B}, {1, -1, -1}, n);
 b= sfps::div(sfps::pow_of_div<Mint>({1}, {1, -1}, n, n), {1, -1, -1}, n);
 for (int i= 0; i <= n; ++i) b[i]-= c[i];
 cout << sample_points_shift<Mint>(b, m) + div_at<Mint>({A, B}, {1, -1, -1}, m) << '\n';
 return 0;
}
