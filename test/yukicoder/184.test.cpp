#define PROBLEM "https://yukicoder.me/problems/no/184"
//行列(F_2)のrankのverify
#include <bits/stdc++.h>

#include "src/LinearAlgebra/LUDecomposition.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  vector<vector<bool>> A(N, vector<bool>(61));
  for (int i = 0; i < N; i++) {
    long long a;
    cin >> a;
    for (int j = 0; j <= 60; j++) A[i][j] = (a >> j) & 1;
  }
  cout << (1ll << LUDecomposition<bool, 112345, 61>(A).rank()) << endl;
  return 0;
}