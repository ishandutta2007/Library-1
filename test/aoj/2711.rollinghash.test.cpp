#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2711"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/CartesianProduct.hpp"
#include "src/String/RollingHash.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<(1ll << 61) - 1>;
  using K = CartesianProduct<Mint, Mint>;
  using RH = RollingHash<K>;
  K base = {get_rand(2, (1ll << 61) - 2), get_rand(2, (1ll << 61) - 2)};
  RH::set_base(base);
  string S;
  cin >> S;
  RH rh(S);
  int Q;
  cin >> Q;
  while (Q--) {
    int l, r, t;
    cin >> l >> r >> t, l--;
    auto a = rh.sub(l, r - t), b = rh.sub(l + t, r);
    int s = lcp(a, b);
    if (s >= r - (l + t)) {
      cout << "Yes" << '\n';
    } else if (a.get_hash(s + 1) == b.get_hash(s + 1)) {
      cout << "Yes" << '\n';
    } else if (s + t < r - l - t &&
               a.get_hash(s + 1, s + t) == b.get_hash(s + 1, s + t) &&
               a.get_hash(s + t + 1) == b.get_hash(s + t + 1) &&
               S[l + s] == S[l + s + t + t]) {
      cout << "Yes" << '\n';
    } else {
      cout << "No" << '\n';
    }
  }
  return 0;
}