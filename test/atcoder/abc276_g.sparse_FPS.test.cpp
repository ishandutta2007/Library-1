#define PROBLEM "https://atcoder.jp/contests/abc276/tasks/abc276_g"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  int N, M;
  cin >> N >> M;
  if (M -= --N; M < 0) {
    cout << 0 << '\n';
    return 0;
  }
  auto F = sparse_pow_of_div<Mint>({1, 1}, {1, 0, 0, -1}, N, M + 1);
  for (int i = 1; i <= M; i++) F[i] += F[i - 1];
  for (int i = 1; i <= M; i++) F[i] += F[i - 1];
  cout << F[M] << '\n';
  return 0;
}