#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2711"
#include <iostream>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/Misc/rng.hpp"
#include "src/String/RollingHash.hpp"
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
 int Q;
 cin >> Q;
 while (Q--) {
  int l, r, t;
  cin >> l >> r >> t, l--;
  int n= r - l;
  auto rhT= rh.sub(l, n);
  auto a= rhT.sub(0, n - t), b= rhT.sub(t, n - t);
  int s= lcp(a, b);
  if (s >= n - t) {
   cout << "Yes" << '\n';
   continue;
  }
  if (s < t) {
   if (a.sub(s + 1) == b.sub(s + 1)) {
    cout << "Yes" << '\n';
    continue;
   }
  }
  if (s + t > n - t) {
   cout << (a.sub(s + 1) == b.sub(s + 1) ? "Yes" : "No") << '\n';
   continue;
  }
  bool isok= a.sub(s + 1, t - 1) == b.sub(s + 1, t - 1);
  if (t + s + t < n) {
   isok&= S[l + s] == S[l + t + s + t];
   isok&= a.sub(s + t + 1) == b.sub(s + t + 1);
  }
  cout << (isok ? "Yes" : "No") << '\n';
 }
 return 0;
}