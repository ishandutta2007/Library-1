#define PROBLEM "https://yukicoder.me/problems/no/1019"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/ModInt.hpp"
#include "Math/mobius_inversion.hpp"
#undef call_from_test

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
  int64_t N;
  cin >> N;
  Mint ans = mobius_inversion<Mint>(N, N, g, b, 2);
  ans = Mint(24) * ans - Mint(16);
  cout << ans << endl;
  return 0;
}
