#define PROBLEM "https://yukicoder.me/problems/no/1019"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/Sieve.hpp"
#include "src/Math/multiplicative_and_additive.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  using namespace multiplicative_functions;
  auto F = [](long long N) {
    Mint S = 0;
    for (long long x = sqrt(N); x; x--)
      S += x * (1 + 2 * (long long)sqrt(N - x * x));
    return S;
  };
  long long N;
  cin >> N;
  long long sqrtN = sqrtl(N);
  auto mu = Sieve<>::multiplicative_table<Mint>(sqrtN, Moebius<Mint>::f);
  Mint ans = 0;
  for (int d = 1; d <= sqrtN; d++) ans += F(N / d / d) * d * mu[d];
  cout << ans * 24 - 16 << endl;
  return 0;
}
