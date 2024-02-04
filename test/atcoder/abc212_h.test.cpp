#define PROBLEM "https://atcoder.jp/contests/abc212/tasks/abc212_h"
#include <iostream>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 constexpr int n= 1 << 16;
 int N, K;
 cin >> N >> K;
 vector<Mint> a(n);
 for (int i= 0, A; i < K; ++i) cin >> A, a[A]+= 1;
 sps::hadamard(a);
 for (auto &x: a) x= x == 1 ? N : (x.pow(N) - 1) / (x - 1) * x;
 sps::hadamard(a);
 Mint all= K == 1 ? N : (Mint(K).pow(N) - 1) / (K - 1) * K;
 cout << all - a[0] / n << '\n';
 return 0;
}