#define PROBLEM "https://judge.yosupo.jp/problem/hafnian_of_matrix"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "hafnian.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  int N;
  cin >> N;
  vector<vector<Mint>> a(N, vector<Mint>(N));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) cin >> a[i][j];
  cout << hafnian(a) << '\n';
  return 0;
}