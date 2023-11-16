#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2711"
#include <iostream>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Math/CartesianProduct.hpp"
#include "src/Misc/rng.hpp"
#include "src/String/RollingHash.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 string S;
 cin >> S;
 RH rh(S);
 int Q;
 cin >> Q;
 while (Q--) {
  int l, r, t;
  cin >> l >> r >> t, l--;
  auto a= rh.sub(l, r - l - t), b= rh.sub(l + t, r - l - t);
  int s= lcp(a, b);
  if (s >= r - (l + t)) {
   cout << "Yes" << '\n';
  } else if (a.sub(s + 1) == b.sub(s + 1)) {
   cout << "Yes" << '\n';
  } else if (s + t < r - l - t && a.sub(s + 1, t - 1) == b.sub(s + 1, t - t) && a.sub(s + t + 1) == b.sub(s + t + 1) && S[l + s] == S[l + s + t + t]) {
   cout << "Yes" << '\n';
  } else {
   cout << "No" << '\n';
  }
 }
 return 0;
}