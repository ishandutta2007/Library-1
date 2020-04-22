#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/Matrix.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int N;
  cin >> N;
  Matrix<Mint> A(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) cin >> A[i][j];
  cout << A.det() << endl;
  return 0;
}