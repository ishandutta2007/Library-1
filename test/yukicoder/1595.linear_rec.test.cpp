// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1595
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<10>;
 long long p, q, r, K;
 cin >> p >> q >> r >> K;
 cout << linear_recurrence<Mint>({1, 1, 1}, {p, q, r}, K - 1) << '\n';
 return 0;
}