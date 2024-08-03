// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc272/tasks/abc272_f
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
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
 string S, T;
 cin >> S >> T;
 S+= S, T+= T;
 RH rhS(S), rhT(T);
 vector<RH> rh(N);
 for (int i= 0; i < N; ++i) rh[i]= rhS.sub(i, N);
 stable_sort(rh.begin(), rh.end());
 long long ans= 0;
 for (int i= 0; i < N; ++i) ans+= upper_bound(rh.begin(), rh.end(), rhT.sub(i, N)) - rh.begin();
 cout << ans << '\n';
 return 0;
}