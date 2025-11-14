// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc208/tasks/abc208_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/sample_points_shift.hpp"
#include "src/NumberTheory/enumerate_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 long long N, M, K;
 cin >> N >> M >> K;
 auto y= pow_table<Mint>(M + K, K);
 for (int i= M; i--;)
  for (int j= 0; j < M + K; ++j) y[j + 1]+= y[j];
 cout << sample_points_shift<Mint>(y, N) << '\n';
 return 0;
}