// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "mylib/algebra/ModInt.hpp"
#include "mylib/misc/Pointwise.hpp"
#include "mylib/misc/rng.hpp"
#include "mylib/string/RollingHash.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<99824435>;
 using K= Pointwise<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 string T, P;
 cin >> T >> P;
 RH rt(T), rp(P);
 int N= P.length(), M= T.length();
 auto hash= rp.hash();
 for(int i= 0; i + N <= M; i++)
  if(rt.sub(i, N).hash() == hash) cout << i << '\n';
 return 0;
}
