#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"
#include <bits/stdc++.h>

#include "src/LinearAlgebra/LUDecomposition.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector A(N, vector<Mint>(N));
 for (int i= 0; i < N; i++)
  for (int j= 0; j < N; j++) cin >> A[i][j];
 cout << LUDecomposition(A).det() << endl;
 return 0;
}