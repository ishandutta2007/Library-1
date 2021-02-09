#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/GaussianElimination.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using GE = GaussianElimination;
  int N;
  cin >> N;
  vector<vector<Mint>> A(N, vector<Mint>(N));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) cin >> A[i][j];
  cout << GE::row_reduction(A).second << endl;
  return 0;
}