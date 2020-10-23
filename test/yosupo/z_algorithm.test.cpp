#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"
#include <bits/stdc++.h>
#include "src/String/z_algorithm.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  string s;
  cin >> s;
  vector<int> ans = z_algorithm(s);
  for (int i = 0; i < s.size(); i++) {
    cout << (i ? " " : "") << ans[i];
  }
  cout << endl;
  return 0;
}