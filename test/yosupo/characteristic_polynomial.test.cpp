#define PROBLEM "https://judge.yosupo.jp/problem/characteristic_polynomial"

#include <bits/stdc++.h>
#include "src/LinearAlgebra/characteristic_polynomial.hpp"
#include "src/Math/ModInt.hpp"

using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<vector<Mint>> a(N, vector<Mint>(N));
 for (int i= 0; i < N; i++)
  for (int j= 0; j < N; j++) cin >> a[i][j];
 auto p= characteristic_polynomial(a);
 for (int i= 0; i <= N; i++) cout << (i ? " " : "") << p[i];
 cout << '\n';
 return 0;
}