#define PROBLEM "https://yukicoder.me/problems/no/803"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/Matrix_mod2.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M, X;
  cin >> N >> M >> X;
  Matrix_mod2 A(30 + M, N);
  vector<int> b(30 + M);
  for (int i = 0; i < 30; i++) b[i] = (X >> i) & 1;
  for (int j = 0; j < N; j++) {
    int a;
    cin >> a;
    for (int i = 0; i < 30; i++) A[i][j] = (a >> i) & 1;
  }
  for (int i = 0; i < M; i++) {
    int l, r;
    cin >> b[30 + i] >> l >> r;
    for (int j = l - 1; j <= r - 1; j++) A[30 + i][j] = 1;
  }
  auto ans = Matrix_mod2::linear_equations(A, b);
  if (ans.first.size())
    cout << ModInt<int(1e9 + 7)>(2).pow(ans.second.size()) << endl;
  else
    cout << 0 << endl;
  return 0;
}