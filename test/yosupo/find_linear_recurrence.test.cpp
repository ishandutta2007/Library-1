#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/ModInt.hpp"
#include "Math/berlekamp_massey.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int N;
  cin >> N;
  vector<Mint> a(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  vector<Mint> c = berlekamp_massey(a);
  int d = c.size();
  cout << d << endl;
  for (int i = 0; i < d; i++) cout << (i ? " " : "") << c[d - 1 - i];
  cout << endl;
  return 0;
}