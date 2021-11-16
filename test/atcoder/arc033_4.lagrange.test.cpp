#define PROBLEM "https://atcoder.jp/contests/arc033/tasks/arc033_4"

#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/lagrange_interpolation.hpp"

using namespace std;
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<int(1e9 + 7)>;
  int N;
  cin >> N;
  vector<Mint> A(N + 1);
  for (int i = 0; i <= N; i++) cin >> A[i];
  int T;
  cin >> T;
  cout << lagrange_interpolation<Mint>(A, T) << '\n';
  return 0;
}