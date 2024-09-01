// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc234/tasks/abc234_g
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
 CartesianTree ct1(A), ct2(A, false);
 BinaryIndexedTree_RangeAdd<Mint> dp(N + 1);
 dp.add_range(0, 1, 1);
 for (int i= 0; i < N; ++i) {
  auto [l1, r1]= ct1.range(i);
  dp.add_range(i + 1, r1 + 1, -dp.sum(l1, i + 1) * A[i]);
  auto [l2, r2]= ct2.range(i);
  dp.add_range(i + 1, r2 + 1, dp.sum(l2, i + 1) * A[i]);
 }
 cout << dp[N] << '\n';
 return 0;
}