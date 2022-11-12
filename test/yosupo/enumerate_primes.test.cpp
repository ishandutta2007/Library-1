#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_primes"
#include <bits/stdc++.h>
#include "src/Math/Sieve.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using S = Sieve<int(5e8 + 10)>;
  int N, A, B;
  cin >> N >> A >> B;
  auto ps = S::get_primes(N);
  int pi = ps.size();
  vector<int> ans;
  for (int i = 0; A * i + B < pi; i++) ans.push_back(ps[A * i + B]);
  int X = ans.size();
  cout << pi << " " << X << '\n';
  for (int i = 0; i < X; i++) cout << ans[i] << " \n"[i == X - 1];
  return 0;
}