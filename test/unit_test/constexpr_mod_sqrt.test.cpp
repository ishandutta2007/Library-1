#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <iostream>
#include "src/Math/mod_sqrt.hpp"
using namespace std;
constexpr auto f= mod_sqrt(4, 1000000007);
static_assert(f == 2);
constexpr auto g= mod_sqrt(3, 998244353);
static_assert(g == -1);
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int A, B;
 cin >> A >> B;
 cout << A + B << '\n';
 return 0;
}