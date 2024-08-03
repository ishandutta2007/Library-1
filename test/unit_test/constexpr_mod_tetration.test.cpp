// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/aplusb
#include <iostream>
#include "src/Math/mod_tetration.hpp"
using namespace std;
constexpr auto f= mod_tetration(2, 3, 1000000007);
static_assert(f == 16);
constexpr auto g= mod_tetration(2, 10000, 998244353);
static_assert(g == 220050301);
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int A, B;
 cin >> A >> B;
 cout << A + B << '\n';
 return 0;
}