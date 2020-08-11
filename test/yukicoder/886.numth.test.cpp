#define PROBLEM "https://yukicoder.me/problems/no/886"

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
  int H, W;
  cin >> H >> W;
  if (H < W) swap(H, W);
  init(H + 1);
  vector<Mint> h(H + 1), w(H + 1);
  for (int i = 1; i < H; i++) h[i] = H - i;
  for (int i = 1; i < W; i++) w[i] = W - i;
  auto a = gcd_convolution(h, w);
  Mint HH(H), WW(W);
  Mint ans = Mint(2) * a[1] + WW * (HH - 1) + HH * (WW - 1);
  cout << ans << endl;
  return 0;
}
