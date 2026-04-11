// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/zalgorithm
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "mylib/Math/ModInt.hpp"
#include "mylib/Misc/Pointwise.hpp"
#include "mylib/Misc/rng.hpp"
#include "mylib/String/RollingHash.hpp"
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
 RH rh(S);
 int N= S.length();
 for (int i= 0; i < N; i++) cout << lcp(rh, rh.sub(i)) << " \n"[i == N - 1];
 return 0;
}