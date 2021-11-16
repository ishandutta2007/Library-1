#define PROBLEM "https://atcoder.jp/contests/arc033/tasks/arc033_4"

#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/SubproductTree.hpp"

using namespace std;
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<int(1e9 + 7)>;
  int N;
  cin >> N;
  vector<Mint> x(N + 1), A(N + 1);
  for (int i = 0; i <= N; i++) cin >> A[i];
  iota(x.begin(), x.end(), 0);
  int T;
  cin >> T;
  cout << SubproductTree<Mint>(x).interpolate(A).eval(T) << '\n';
  return 0;
}