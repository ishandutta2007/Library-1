// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0168
#include <iostream>
#include "src/Math/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 for (int n; cin >> n && n;) cout << (linear_recurrence<int>({1, 1, 1}, {1, 1, 2}, n) + 3649) / 3650 << '\n';
 return 0;
}
