#define PROBLEM "https://yukicoder.me/problems/no/1019"
#include <iostream>
#include "src/Math/DirichletSeries.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 uint64_t N;
 cin >> N;
 auto F= [](uint64_t n) {
  Mint S= 0;
  for (uint64_t x= sqrt(n); x; --x) S+= x * (1 + 2 * (uint64_t)sqrt(n - x * x));
  return S;
 };
 auto gsum= [&](uint64_t n) {
  uint64_t a= sqrt(n);
  return a * (a + 1) / 2;
 };
 DirichletSeries<Mint> H(N, F), G(N, gsum);
 Mint ans= (H / G).sum();
 ans= ans * 24 - 16;
 cout << ans << '\n';
 return 0;
}