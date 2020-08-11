#define PROBLEM "https://yukicoder.me/problems/no/644"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/ModInt.hpp"
#include "src/Math/number_theory.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<int(1e9 + 7)>;
  using namespace number_theory;
  int N, M;
  cin >> N >> M;
  init(N);
  auto phi = get_phi<Mint>(N);
  Mint ans = 0;
  for (int i = 2; i <= N / M; i++) ans += phi[i];
  for (int i = 1; i <= N - 2; i++) ans *= Mint(i);
  ans *= Mint(2);
  cout << ans << endl;
  return 0;
}