#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <iostream>
#include "src/Math/ModInt.hpp"
using namespace std;
using Mint= ModInt<1000000007>;
constexpr auto f= Mint(3).pow(1000000004);
static_assert(f == 111111112);
constexpr auto g= Mint(2) / 3;
static_assert(g == 666666672);
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int A, B;
 cin >> A >> B;
 cout << A + B << '\n';
 return 0;
}