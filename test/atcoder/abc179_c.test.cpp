#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_c"

// O(√N)

#include <bits/stdc++.h>
#include "src/Math/dujiao_sieve.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  auto gsum = [](long long n) { return n; };
  long long N;
  cin >> N;
  cout << dirichlet_mul_sum<long long>(N - 1, gsum, gsum) << '\n';
  return 0;
}