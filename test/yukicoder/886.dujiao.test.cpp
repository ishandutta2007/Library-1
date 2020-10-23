#define PROBLEM "https://yukicoder.me/problems/no/886"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/dujiao_sieve.hpp"
using namespace std;

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
  map<pair<int64_t, int64_t>, Mint> memo_1, memo_w, memo_h, memo_hw;
  int64_t H, W;
  cin >> H >> W;
  Mint ans_1 = dujiao_sieve<Mint>(H - 1, W - 1, g_1, b_0, memo_1);
  Mint ans_w = dujiao_sieve<Mint>(H - 1, W - 1, g_w, b_1, memo_w);
  Mint ans_h = dujiao_sieve<Mint>(H - 1, W - 1, g_h, b_1, memo_h);
  Mint ans_hw = dujiao_sieve<Mint>(H - 1, W - 1, g_hw, b_2, memo_hw);
  Mint MH(H), MW(W);
  Mint ans = MH * MW * ans_1 - MH * ans_w - MW * ans_h + ans_hw;
  ans *= (2);
  ans += (MH - 1) * MW + (MW - 1) * MH;
  cout << ans << endl;
  return 0;
}
