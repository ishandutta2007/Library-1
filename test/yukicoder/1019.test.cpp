#define PROBLEM "https://yukicoder.me/problems/no/1019"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/NumberTheory.hpp"
#include "src/Math/multiplicative_and_additive.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  using NT = NumberTheory;
  using namespace multiplicative_functions;
  auto hsum = [](long long N) {
    long long x_max = sqrt(N);
    Mint S = 0;
    for (long long x = 1; x <= x_max; x++) {
      long long y_max = sqrt(N - x * x);
      S += x * (1 + 2 * y_max);
    }
    return S;
  };
  long long N;
  cin >> N;
  long long sqrtN = sqrtl(N);
  auto mu = NT::multiplicative_table<Mint>(sqrtN, Moebius<Mint>::f);
  Mint ans = 0;
  for (int d = 1; d <= sqrtN; d++) ans += hsum(N / d / d) * d * mu[d];
  ans = Mint(24) * ans - Mint(16);
  cout << ans << endl;
  return 0;
}
