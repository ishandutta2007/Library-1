#define PROBLEM "https://yukicoder.me/problems/no/886"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/ModInt.hpp"
#include "Math/mobius_inversion.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<int(1e9 + 7)>;
  auto g_1 = [](int64_t H, int64_t W) { return Mint(H * W); };
  auto g_w
      = [](int64_t H, int64_t W) { return Mint(H) * Mint(W * (W + 1) / 2); };
  auto g_h
      = [](int64_t H, int64_t W) { return Mint(H * (H + 1) / 2) * Mint(W); };
  auto g_hw = [](int64_t H, int64_t W) {
    return Mint(H * (H + 1) / 2) * Mint(W * (W + 1) / 2);
  };
  auto b_0 = [](int64_t d) { return Mint(d); };
  auto b_1 = [](int64_t d) { return Mint(d * (d + 1) / 2); };
  auto b_2 = [](int64_t d) {
    return Mint(d * (d + 1) / 2) * Mint(2 * d + 1) / Mint(3);
  };
  int64_t H, W;
  cin >> H >> W;
  Mint ans_1 = mobius_inversion<Mint>(H - 1, W - 1, g_1, b_0);
  Mint ans_w = mobius_inversion<Mint>(H - 1, W - 1, g_w, b_1);
  Mint ans_h = mobius_inversion<Mint>(H - 1, W - 1, g_h, b_1);
  Mint ans_hw = mobius_inversion<Mint>(H - 1, W - 1, g_hw, b_2);
  Mint MH(H), MW(W);
  Mint ans = MH * MW * ans_1 - MH * ans_w - MW * ans_h + ans_hw;
  ans *= (2);
  ans += (MH - 1) * MW + (MW - 1) * MH;
  cout << ans << endl;
  return 0;
}
