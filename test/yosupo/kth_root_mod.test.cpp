#define PROBLEM "https://judge.yosupo.jp/problem/kth_root_mod"
#include <bits/stdc++.h>
#include "src/Math/mod_kth_root.hpp"
using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int K, Y, P;
    cin >> K >> Y >> P;
    cout << mod_kth_root(Y, K, P) << '\n';
  }
  return 0;
}