#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2614"
#include <iostream>
#include <set>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Misc/rng.hpp"
#include "src/Math/CartesianProduct.hpp"
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
 string S, T;
 cin >> S >> T;
 RH rh(S);
 int N= S.length(), M= T.length();
 multiset<K> hashes;
 for (int i= 0; i + M <= N; i++) hashes.insert(rh.get_hash(i, i + M));
 K thash= RH(T).get_hash();
 int ans= 0;
 for (int i= 0; i < M; i++) {
  for (char c= 'a'; c <= 'z'; c++)
   if (c != T[i]) {
    thash-= RH::base_pow(M - 1 - i) * T[i];
    thash+= RH::base_pow(M - 1 - i) * c;
    ans+= hashes.count(thash);
    thash-= RH::base_pow(M - 1 - i) * c;
    thash+= RH::base_pow(M - 1 - i) * T[i];
   }
  for (char c= 'A'; c <= 'Z'; c++)
   if (c != T[i]) {
    thash-= RH::base_pow(M - 1 - i) * T[i];
    thash+= RH::base_pow(M - 1 - i) * c;
    ans+= hashes.count(thash);
    thash-= RH::base_pow(M - 1 - i) * c;
    thash+= RH::base_pow(M - 1 - i) * T[i];
   }
 }
 cout << ans << '\n';
 return 0;
}