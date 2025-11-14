// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc198/tasks/abc198_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// OEIS A054473, 実装サボり
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/berlekamp_massey.hpp"
#include "src/Math/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 vector<Mint> a= {1, 1, 3, 5, 10, 15, 29, 41, 68, 98, 147, 202, 291, 386, 528, 688, 906, 1151, 1480, 1841, 2310, 2833, 3484, 4207, 5099, 6076, 7259, 8562, 10104, 11796};
 long long S;
 cin >> S;
 cout << linear_recurrence(berlekamp_massey(a), a, S - 6) << '\n';
 return 0;
}