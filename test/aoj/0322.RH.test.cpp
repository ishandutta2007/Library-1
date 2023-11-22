#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/0322"

// multiset 使ったら MLE になった

#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Math/CartesianProduct.hpp"
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9) + 7>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 int N, M;
 cin >> N >> M;
 vector<vector<K>> dic(201), dic_p(201), dic_s(201);
 for (int i= 0; i < N; ++i) {
  string word;
  cin >> word;
  RH rh(word);
  int n= word.length();
  for (int j= 1; j <= n; ++j) dic_p[j].push_back(rh.sub(0, j).hash());
  for (int j= 0; j < n; ++j) dic_s[n - j].push_back(rh.sub(j).hash());
  dic[n].push_back(rh.hash());
 }
 for (int n= 1; n <= 200; ++n) {
  sort(dic_p[n].begin(), dic_p[n].end());
  sort(dic_s[n].begin(), dic_s[n].end());
  sort(dic[n].begin(), dic[n].end());
 }
 auto count= [](vector<K> *dc, K x) { return upper_bound(dc->begin(), dc->end(), x) - lower_bound(dc->begin(), dc->end(), x); };
 RH chars("abcdefghijklmnopqrstuvwxyz");
 for (int i= 0; i < M; ++i) {
  string slate;
  cin >> slate;
  int m= slate.length();
  int wi= -1;
  for (int j= 0; j < m; ++j)
   if (slate[j] == '?') wi= j;
  RH rh(slate);
  vector<K> *dc;
  if (slate[0] == '*') {
   rh= rh.sub(1), --m;
   if (wi != -1) --wi;
   dc= &dic_s[m];
  } else if (slate.back() == '*') {
   rh= rh.sub(0, m - 1), --m;
   dc= &dic_p[m];
  } else {
   dc= &dic[m];
  }
  int ans= 0;
  if (wi == -1) ans= count(dc, rh.hash());
  else
   for (int c= 26; c--;) ans+= count(dc, concat_hash(rh.sub(0, wi), chars.sub(c, 1), rh.sub(wi + 1)));
  cout << ans << '\n';
 }
 return 0;
}