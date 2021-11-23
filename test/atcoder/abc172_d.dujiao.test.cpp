#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"

// O(√N)

#include <bits/stdc++.h>
#include "src/Math/dujiao_sieve.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  auto gsum = [](long long n) { return n * (n + 1) / 2; };
  long long N;
  cin >> N;
  cout << dirichlet_mul_sum<long long>(N, gsum, gsum) << '\n';
  return 0;
}