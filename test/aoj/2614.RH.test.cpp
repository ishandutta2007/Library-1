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
 using Mint= ModInt<998244353>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K, char>;
 RH::init({rng(), rng()});
 string S, T;
 cin >> S >> T;
 RH rh(S), rhT(T);
 int N= S.length(), M= T.length();
 multiset<K> hs;
 for (int i= 0; i + M <= N; i++) hs.insert(rh.sub(i, M).hash());
 vector<char> chars(52);
 for (int i= 0; i < 26; i++) chars[i]= 'a' + i;
 for (int i= 0; i < 26; i++) chars[i + 26]= 'A' + i;
 RH rhC(chars);
 int ans= 0;
 for (int i= 0; i < M; ++i)
  for (int j= 0; j < 52; ++j)
   if (T[i] != chars[j]) ans+= hs.count(concat_hash(rhT.sub(0, i), rhC.sub(j, 1), rhT.sub(i + 1)));
 cout << ans << '\n';
 return 0;
}