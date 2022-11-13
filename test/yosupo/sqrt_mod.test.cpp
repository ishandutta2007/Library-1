#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
// sqrt の verify
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = RuntimeModInt<int>;
  int T;
  cin >> T;
  while (T--) {
    int Y, P;
    cin >> Y >> P;
    Mint::set_mod(P);
    auto ans = Mint(Y).sqrt();
    if (ans * ans == Y)
      cout << ans << '\n';
    else
      cout << -1 << '\n';
  }
  return 0;
}