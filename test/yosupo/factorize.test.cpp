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
    long long a;
    cin >> a;
    vector<long long> ans;
    for (auto& [p, e] : Factors(a))
      for (int i = 0; i < e; i++) ans.push_back(p);
    cout << ans.size();
    for (auto x : ans) cout << " " << x;
    cout << '\n';
  }
  return 0;
}