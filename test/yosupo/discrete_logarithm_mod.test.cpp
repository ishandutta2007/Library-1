#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"
#include <bits/stdc++.h>
#include "src/Math/discrete_log.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int T;
  cin >> T;
  while (T--) {
    int X, Y, M;
    cin >> X >> Y >> M;
    cout << discrete_log(X, Y, M) << '\n';
  }
  return 0;
}