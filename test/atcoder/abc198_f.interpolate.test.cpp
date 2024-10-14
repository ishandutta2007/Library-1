// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc198/tasks/abc198_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
#include "src/Math/sample_points_shift.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 auto a= sfps::pow_of_div<Mint>({1}, {1, -1}, 6, 72);
 auto b= sfps::pow_of_div<Mint>({1}, {1, 0, -1}, 3, 72);
 for (int i= 72; i--;) a[i]+= b[i] * 6;
 b= sfps::pow_of_div<Mint>({1}, {1, 0, 0, -1}, 2, 72);
 for (int i= 72; i--;) a[i]+= b[i] * 8;
 b= sfps::pow_of_div<Mint>({1}, {1, -1}, 2, 72);
 auto c= sfps::div<Mint>(b, {1, 0, -2, 0, 1}, 72);
 for (int i= 72; i--;) a[i]+= c[i] * 3;
 c= sfps::div<Mint>(b, {1, 0, 0, 0, -1}, 72);
 for (int i= 72; i--;) a[i]+= c[i] * 6;
 long long S;
 cin >> S;
 S-= 6;
 vector<Mint> y(6);
 for (int i= 0; i < 6; ++i) y[i]= a[i * 12 + S % 12];
 cout << sample_points_shift<Mint>(y, S / 12) / 24 << '\n';
 return 0;
}