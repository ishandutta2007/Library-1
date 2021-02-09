#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/GaussianElimination.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using GE = GaussianElimination;
  int N, M;
  cin >> N >> M;
  vector<vector<Mint>> A(N, vector<Mint>(M));
  vector<Mint> b(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++) cin >> A[i][j];
  for (int i = 0; i < N; i++) cin >> b[i];
  auto [c, d] = GE::linear_equation(A, b);
  if (!c.size())
    cout << -1 << endl;
  else {
    cout << d.size() << endl;
    for (int j = 0; j < M; j++) { cout << (j ? " " : "") << c[j]; }
    cout << endl;
    for (int i = 0; i < d.size(); i++) {
      for (int j = 0; j < M; j++) { cout << (j ? " " : "") << d[i][j]; }
      cout << endl;
    }
  }
  return 0;
}