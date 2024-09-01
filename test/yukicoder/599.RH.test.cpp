// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/599
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9) + 7>;
 using K= Pointwise<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 string S;
 cin >> S;
 int n= S.length();
 RH rh(S);
 vector<Mint> dp(n / 2 + 1);
 dp[0]= 1;
 Mint ans= 0;
 for (int i= 0; i <= n / 2; ++i) {
  for (int j= 0; j < i; ++j)
   if (rh.sub(j, i - j) == rh.sub(n - i, i - j)) dp[i]+= dp[j];
  ans+= dp[i];
 }
 cout << ans;
 return 0;
}