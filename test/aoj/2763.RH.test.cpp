#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2763"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Math/CartesianProduct.hpp"
#include "src/Misc/rng.hpp"
#include "src/String/RollingHash.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9) + 7>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 string S;
 cin >> S;
 int n= S.length();
 RH rh(S);
 int ans= -1;
 for (int i= 1; i * 3 < n; ++i) {
  if ((n & 1) != (i & 1)) continue;
  int j= (n - i) / 2;
  if (rh.sub(0, j) != rh.sub(j, j)) continue;
  if (rh.sub(0, i) != rh.sub(j + j, i)) continue;
  ans= j;
 }
 cout << (ans < 0 ? "mitomerarenaiWA" : "Love " + S.substr(0, ans) + "!") << '\n';
 return 0;
}