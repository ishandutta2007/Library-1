#define PROBLEM "https://yukicoder.me/problems/no/644"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/NumberTheory.hpp"
#include "src/Math/multiplicative_functions.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<int(1e9 + 7)>;
  using NT = NumberTheory;
  using namespace multiplicative_functions;
  int N, M;
  cin >> N >> M;
  auto phi = NT::multiplicative_table<Mint>(N, Totient<Mint>::f);
  Mint ans = 0;
  for (int i = 2; i <= N / M; i++) ans += phi[i];
  for (int i = 1; i <= N - 2; i++) ans *= Mint(i);
  ans *= Mint(2);
  cout << ans << endl;
  return 0;
}