#define PROBLEM "https://yukicoder.me/problems/no/1019"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/dujiao_sieve.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  auto g = [](int64_t N, int64_t dummy) {
    int64_t x_max = sqrt(N);
    Mint S = 0;
    for (int64_t x = 1; x <= x_max; x++) {
      int64_t y_max = sqrt(N - x * x);
      S += x * (1 + 2 * y_max);
    }
    return S;
  };
  auto b = [](int64_t d) { return Mint(d * (d + 1) / 2); };
  map<pair<int64_t, int64_t>, Mint> memo;
  int64_t N;
  cin >> N;
  Mint ans = dujiao_sieve<Mint>(N, N, g, b, memo, 2);
  ans = Mint(24) * ans - Mint(16);
  cout << ans << endl;
  return 0;
}
