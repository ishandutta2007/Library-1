// competitive-verifier: PROBLEM https://www.hackerrank.com/challenges/save-humanity
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include <vector>
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
 int t;
 cin >> t;
 auto check= [&](RH a, RH b) {
  int l= lcp(a, b);
  return l == b.length() || a.sub(l + 1) == b.sub(l + 1);
 };
 while (t--) {
  string p, v;
  cin >> p >> v;
  RH rhp(p), rhv(v);
  int n= p.size(), m= v.size();
  vector<int> ans;
  for (int i= 0; i + m <= n; ++i)
   if (check(rhp.sub(i, m), rhv)) ans.push_back(i);
  if (ans.empty()) cout << "No Match!" << '\n';
  else
   for (int i= 0, e= ans.size(); i < e; ++i) cout << ans[i] << " \n"[i + 1 == e];
 }
 return 0;
}