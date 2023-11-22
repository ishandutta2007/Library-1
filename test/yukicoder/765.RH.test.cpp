#define PROBLEM "https://yukicoder.me/problems/no/765"
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
 string S;
 cin >> S;
 int N= S.length();
 RH rh(S);
 reverse(S.begin(), S.end());
 RH rhrv(S);
 int ans= 0;
 for (int i= 0; i < N + N - 1; ++i) {
  int j= (i + 1) / 2, k= (N + N - i - 1) / 2;
  int n= lcp(rh.sub(j), rhrv.sub(k));
  int nn= n * 2 - !(i & 1);
  if (nn == N) {
   ans= N - 1;
   break;
  }
  if (j + n + 1 <= N) {
   int m= lcp(rh.sub(j + n + 1), rhrv.sub(k + n));
   ans= max(ans, nn + m * 2);
  }
  if (k + n + 1 <= N) {
   int m= lcp(rh.sub(j + n), rhrv.sub(k + n + 1));
   ans= max(ans, nn + m * 2);
  }
 }
 cout << ans << '\n';
 return 0;
}