// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/665
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/sample_points_shift.hpp"
using namespace std;
//より厳しい制約: http://codeforces.com/contest/622/problem/F

signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 long long n, k;
 cin >> n >> k;
 vector<Mint> y(k + 2, 0);
 for (int i= 1; i <= k + 1; i++) {
  y[i]= y[i - 1] + Mint(i).pow(k);
 }
 cout << sample_points_shift<Mint>(y, n)[0] << endl;
 return 0;
}