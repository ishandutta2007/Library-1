#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"
#include <bits/stdc++.h>
#include "src/Math/Factors.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int Q;
  cin >> Q;
  while (Q--) {
    long long p;
    cin >> p;
    cout << primitive_root(p) << '\n';
  }
  return 0;
}