#define PROBLEM "https://judge.yosupo.jp/problem/counting_primes"
#include <bits/stdc++.h>
#include "src/Math/multiplicative_sum.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  long long N;
  cin >> N;
  cout << prime_count(N) << '\n';
  return 0;
}