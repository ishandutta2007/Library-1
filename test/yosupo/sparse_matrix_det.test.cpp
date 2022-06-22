#define PROBLEM "https://judge.yosupo.jp/problem/sparse_matrix_det"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/SparseSquareMatrix.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  int N, K;
  cin >> N >> K;
  SparseSquareMatrix<Mint> M(N);
  for (int i = 0; i < K; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    M.add_component(a, b, c);
  }
  cout << M.det() << '\n';
  return 0;
}