#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_palindromes"
#include <iostream>
#include <string>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= Pointwise<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 string S;
 cin >> S;
 int N= S.length();
 RH rh(S);
 reverse(S.begin(), S.end());
 RH rhrv(S);
 for (int i= 0; i < N + N - 1; ++i) {
  int n= lcp(rh.sub((i + 1) / 2), rhrv.sub((N + N - i - 1) / 2));
  cout << n * 2 - !(i & 1) << " \n"[i == N + N - 2];
 }
 return 0;
}