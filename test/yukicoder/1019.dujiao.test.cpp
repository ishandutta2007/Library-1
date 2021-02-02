#define PROBLEM "https://yukicoder.me/problems/no/1019"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/dujiao_sieve.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  auto hsum = [](long long N) {
    long long x_max = sqrt(N);
    Mint S = 0;
    for (long long x = 1; x <= x_max; x++) {
      long long y_max = sqrt(N - x * x);
      S += x * (1 + 2 * y_max);
    }
    return S;
  };
  auto gsum = [](long long n) {
    long long d = sqrt(n);
    return Mint(d * (d + 1) / 2);
  };
  long long N;
  cin >> N;
  Mint ans = dirichlet_inv_sum<Mint>(N, gsum, hsum);
  ans = Mint(24) * ans - Mint(16);
  cout << ans << endl;
  return 0;
}
