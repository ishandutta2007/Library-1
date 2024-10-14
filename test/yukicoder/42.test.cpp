// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/42
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sample_points_shift.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 9)>;
 const int SIZE= 500 * 6 + 10;
 Mint dp[SIZE]= {1};
 for (int x: {1, 5, 10, 50, 100, 500})
  for (int i= x; i < SIZE; i++) dp[i]+= dp[i - x];
 int T;
 cin >> T;
 while (T--) {
  long long M;
  cin >> M;
  vector<Mint> y(6);
  for (int i= 0; i <= 5; i++) y[i]= dp[M % 500 + i * 500];
  cout << sample_points_shift<Mint>(y, M / 500) << '\n';
 }
 return 0;
}