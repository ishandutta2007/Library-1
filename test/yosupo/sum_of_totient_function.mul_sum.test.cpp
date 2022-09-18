#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_totient_function"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/prime_count.hpp"
#include "src/Math/multiplicative_and_additive.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = StaticModInt<998244353>;
  using namespace multiplicative_functions;
  long long N;
  cin >> N;
  cout << multiplicative_sum<Mint>(N, Totient<Mint>::f, Totient<Mint>::poly())
       << '\n';
  return 0;
}