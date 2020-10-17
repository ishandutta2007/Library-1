#define PROBLEM "https://yukicoder.me/problems/no/1145"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  int N, M;
  cin >> N >> M;
  deque<FPS> fs;
  for (int i = 0; i < N; i++) {
    Mint A;
    cin >> A;
    fs.push_back(FPS({1, -A}));
  }
  for (; fs.size() > 1; fs.pop_front(), fs.pop_front())
    fs.push_back(fs[0] * fs[1]);
  auto ans = -fs[0].log(M + 1);
  for (int i = 1; i <= M; i++) cout << (i - 1 ? " " : "") << ans[i] * i;
  cout << endl;
  return 0;
}