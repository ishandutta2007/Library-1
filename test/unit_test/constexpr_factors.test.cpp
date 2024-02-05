#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <iostream>
#include "src/Math/Factors.hpp"
using namespace std;
constexpr auto f= Factors(2 * 2 * 3 * 5);
static_assert(f.size() == 3);
static_assert(f[0].first == 2);
static_assert(f[0].second == 2);
static_assert(f[1].first == 3);
static_assert(f[1].second == 1);
static_assert(f[2].first == 5);
static_assert(f[2].second == 1);
constexpr int n= totient(100);
static_assert(n == 40);
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int A, B;
 cin >> A >> B;
 cout << A + B << '\n';
 return 0;
}