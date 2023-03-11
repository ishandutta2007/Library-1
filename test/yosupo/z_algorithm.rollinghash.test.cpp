#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"
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
 using Mint= ModInt<(1ll << 61) - 1>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K>;
 K base= {rng(2, Mint::mod() - 1), rng(2, Mint::mod() - 1)};
 RH::set_base(base);
 string S;
 cin >> S;
 RH rh(S);
 int N= S.length();
 for (int i= 0; i < N; i++) {
  cout << lcp(rh, rh.sub(i, N)) << " \n"[i == N - 1];
 }
 return 0;
}