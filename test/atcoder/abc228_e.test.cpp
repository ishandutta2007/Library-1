#define PROBLEM "https://atcoder.jp/contests/abc228/tasks/abc228_e"
#include <bits/stdc++.h>
#include "src/Math/ModIntExp.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModIntExp<998244353>;
  Mint n, k, m;
  cin >> n >> k >> m;
  cout << m.pow(k.pow(n)) << endl;
  return 0;
}