#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/search/titles/2235"
#include <bits/stdc++.h>

#include "src/DataStructure/EulerTourTree.hpp"
#include "src/DataStructure/OnlineDynamicConnectivity.hpp"
using namespace std;
int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  int n, k;
  cin >> n >> k;
  OnlineDynamicConnectivity dicon(n);
  while (k--) {
    int op, u, v;
    cin >> op >> u >> v;
    if (op == 1) dicon.link(u, v);
    if (op == 2) dicon.cut(u, v);
    if (op == 3) cout << (dicon.connected(u, v) ? "YES" : "NO") << '\n';
  }
}