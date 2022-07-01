#define PROBLEM "https://judge.yosupo.jp/problem/tetration_mod"
#include <bits/stdc++.h>
#include "src/Math/tetration.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int T;
  cin >> T;
  while (T--) {
    int A, B, M;
    cin >> A >> B >> M;
    cout << H4(A, B, M) << '\n';
  }
  return 0;
}