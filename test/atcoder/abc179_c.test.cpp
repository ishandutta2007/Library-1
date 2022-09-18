#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_c"
// O(√N)
#include <bits/stdc++.h>
#include "src/Math/DirichletConvSumTable.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  long long N;
  cin >> N;
  auto f = get_1<long long>(N - 1, (int)sqrt(N - 1));
  cout << dirichlet_mul_sum<long long>(f, f) << '\n';
  return 0;
}