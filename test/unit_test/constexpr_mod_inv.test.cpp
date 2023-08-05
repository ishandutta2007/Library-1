#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <iostream>
#include "src/Math/mod_inv.hpp"
using namespace std;
constexpr auto f= mod_inv(2, 1000000007);
static_assert(f == 500000004);
constexpr auto g= mod_inv(3, 998244353);
static_assert(g == 332748118);
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int A, B;
 cin >> A >> B;
 cout << A + B << '\n';
 return 0;
}