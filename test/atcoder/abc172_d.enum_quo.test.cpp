// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc172/tasks/abc172_d
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
 for (auto [q, l, r]: enumerate_quotients(N)) ans+= ((r + 1) * r - (l + 1) * l) / 2 * q * (q + 1) / 2;
 cout << ans << '\n';
 return 0;
}