#define PROBLEM "https://yukicoder.me/problems/no/2454"
#include <iostream>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Math/CartesianProduct.hpp"
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 int T;
 cin >> T;
 while (T--) {
  int N;
  string S;
  cin >> N >> S;
  RH rh(S);
  int ans= 0;
  for (int i= 1; i < N; ++i) ans+= rh.sub(0, i) < rh.sub(i);
  cout << ans << '\n';
 }
 return 0;
}