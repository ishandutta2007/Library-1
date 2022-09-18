#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"
#include <bits/stdc++.h>

#include "src/LinearAlgebra/LUDecomposition.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  int N;
  cin >> N;
  vector<vector<Mint>> A(N, vector<Mint>(N));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) cin >> A[i][j];
  LUDecomposition lu(A);
  auto ans = lu.inverse_matrix();
  if (ans.empty()) {
    cout << "-1" << '\n';
    return 0;
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) cout << (j ? " " : "") << ans[i][j];
    cout << '\n';
  }
  return 0;
}