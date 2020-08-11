#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/Matrix.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int N, M;
  cin >> N >> M;
  Matrix<Mint> A(N, M);
  vector<Mint> b(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++) cin >> A[i][j];
  for (int i = 0; i < N; i++) cin >> b[i];
  vector<Mint> c;
  vector<vector<Mint>> d;
  tie(c, d) = Matrix<Mint>::linear_equations(A, b);
  if (!c.size())
    cout << -1 << endl;
  else {
    cout << d.size() << endl;
    for (int j = 0; j < M; j++) {
      cout << (j ? " " : "") << c[j];
    }
    cout << endl;
    for (int i = 0; i < d.size(); i++) {
      for (int j = 0; j < M; j++) {
        cout << (j ? " " : "") << d[i][j];
      }
      cout << endl;
    }
  }
  return 0;
}