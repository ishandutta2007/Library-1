#define PROBLEM "https://judge.yosupo.jp/problem/two_sat"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/StronglyConnectedComponents.hpp"
#include "Math/TwoSatisfiability.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  string s;
  cin >> s >> s;
  int N, M;
  cin >> N >> M;
  TwoSatisfiability sat(N + 1);
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b >> s;
    if (a < 0) a = sat.neg(-a);
    if (b < 0) b = sat.neg(-b);
    sat.add_or(a, b);
  }
  auto ans = sat.solve();
  cout << "s ";
  if (ans.size()) {
    cout << "SATISFIABLE" << endl;
    cout << "v ";
    for (int i = 1; i <= N; i++) {
      cout << (ans[i] ? i : -i) << " ";
    }
    cout << 0 << endl;
  } else {
    cout << "UNSATISFIABLE" << endl;
  }
  return 0;
}