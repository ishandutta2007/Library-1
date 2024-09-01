// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc115/tasks/arc115_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Misc/CartesianTree.hpp"
#include "src/DataStructure/BinaryIndexedTree_RangeAdd.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 CartesianTree ct(A);
 array dp{BinaryIndexedTree_RangeAdd<Mint>(N + 1), BinaryIndexedTree_RangeAdd<Mint>(N + 1)};
 dp[0].add_range(0, 1, 1);
 for (int i= 0; i < N; ++i) {
  auto [l, r]= ct.range(i);
  for (int b= 0; b < 2; ++b) dp[!b].add_range(i + 1, r + 1, dp[b].sum(l, i + 1) * A[i]);
 }
 bool b= N & 1;
 cout << dp[b][N] - dp[!b][N] << '\n';
 return 0;
}