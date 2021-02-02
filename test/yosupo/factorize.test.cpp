#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include <bits/stdc++.h>
#include "src/Math/factorize.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int Q;
  cin >> Q;
  while (Q--) {
    unsigned long long a;
    cin >> a;
    auto ans = factorize(a);
    cout << ans.size();
    for (auto x : ans) cout << " " << x;
    cout << '\n';
  }
  return 0;
}