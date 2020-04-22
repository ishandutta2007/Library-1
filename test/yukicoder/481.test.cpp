#define PROBLEM "https://yukicoder.me/problems/481"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/BitMatrix.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  BitMatrix A(N, 61);
  for (int i = 0; i < N; i++) {
    long long a;
    cin >> a;
    for (int j = 0; j <= 60; j++) A[i][j] = (a >> j) & 1;
  }
  cout << (1ll << A.rank()) << endl;
  return 0;
}