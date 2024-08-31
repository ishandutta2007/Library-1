// competitive-verifier: PROBLEM https://atcoder.jp/contests/agc047/tasks/agc047_b
// competitive-verifier: TLE 1
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "src/String/RollingHash.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= Pointwise<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 int N;
 cin >> N;
 vector<string> S(N);
 for (int i= 0; i < N; ++i) cin >> S[i];
 vector<RH> rh(N);
 for (int i= 0; i < N; ++i) rh[i]= RH(S[i]);
 map<K, vector<int>> mp;
 for (int i= 0; i < N; ++i) {
  int c= S[i][0] - 'a';
  K h= rh[i].sub(1).hash();
  auto &v= mp[h];
  if (v.empty()) v.resize(26);
  v[c]++;
 }
 long long ans= 0;
 for (int i= 0; i < N; ++i) {
  int n= S[i].length();
  bool used[26];
  fill_n(used, 26, false);
  for (int j= 0; j < n; ++j) {
   used[S[i][j] - 'a']= true;
   K h= rh[i].sub(j + 1).hash();
   for (int k= 0; k < 26; ++k)
    if (used[k])
     if (auto it= mp.find(h); it != mp.end()) ans+= it->second[k];
  }
 }
 ans-= N;
 cout << ans << '\n';
 return 0;
}