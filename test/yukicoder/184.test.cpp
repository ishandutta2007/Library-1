#define PROBLEM "https://yukicoder.me/problems/no/184"
//行列(F_2)のrankのverify
#include <bits/stdc++.h>
#include "src/Math/Matrix_mod2.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  Matrix_mod2 A(N, 61);
  for (int i = 0; i < N; i++) {
    long long a;
    cin >> a;
    for (int j = 0; j <= 60; j++) A[i][j] = (a >> j) & 1;
  }
  cout << (1ll << A.rank()) << endl;
  return 0;
}