#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_2_64"
#include <bits/stdc++.h>
#include "src/FFT/convolve.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  vector<std::uint64_t> a(N), b(M);
  for (int i = 0; i < N; i++) cin >> a[i];
  for (int j = 0; j < M; j++) cin >> b[j];
  auto c = convolve(a, b);
  c.resize(N + M - 1);
  for (int k = 0; k < N + M - 1; k++) {
    cout << c[k] << " \n"[k == N + M - 2];
  }
  return 0;
}