// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc179/tasks/abc179_c
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// O(√N)
#include <iostream>
#include "src/NumberTheory/enumerate_quotients.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 long long ans= 0;
 for (auto [q, l, r]: enumerate_quotients(N - 1)) ans+= (r - l) * q;
 cout << ans << '\n';
 return 0;
}