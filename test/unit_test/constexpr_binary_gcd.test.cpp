#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <iostream>
#include "src/Math/binary_gcd.hpp"
using namespace std;
constexpr auto f= binary_gcd(2, 4);
static_assert(f == 2);
constexpr auto g= binary_gcd(1000000007, 1000000009);
static_assert(g == 1);
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int A, B;
 cin >> A >> B;
 cout << A + B << '\n';
 return 0;
}