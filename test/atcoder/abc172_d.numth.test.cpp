#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"

// O(N log log N)

#include <bits/stdc++.h>
#include "src/Math/NumberTheory.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N;
  cin >> N;
  vector<long long> d(N + 1, 1);
  NumberTheory::divisor_zeta(d);
  long long ans = 0;
  for (long long i = 1; i <= N; i++) ans += d[i] * i;
  cout << ans << endl;
  return 0;
}