#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <iostream>
#include "src/Math/is_prime.hpp"
using namespace std;
constexpr auto f= is_prime(1000000007);
static_assert(f == true);
constexpr auto g= is_prime(1729);
static_assert(g == false);
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int A, B;
 cin >> A >> B;
 cout << A + B << '\n';
 return 0;
}