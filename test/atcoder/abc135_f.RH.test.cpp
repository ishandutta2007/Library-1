// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc135/tasks/abc135_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
#include <iostream>
#include <vector>
#include "src/String/RollingHash.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/Misc/rng.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= Pointwise<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 string s, t;
 cin >> s >> t;
 int N= s.length(), M= t.length();
 string ss= s;
 for (int i= (M + N - 1) / N; i--;) ss+= s;
 RH rs(ss), rt(t);
 vector<int> next(N, -1);
 auto th= rt.hash();
 for (int i= 0; i < N; ++i)
  if (rs.sub(i, M).hash() == th) next[i]= (i + M) % N;
 vector<int> dep(N, -1);
 int ans= 0;
 for (int i= 0; i < N; ++i) {
  auto dfs= [&](auto dfs, int v) {
   if (dep[v] != -1) return dep[v];
   int u= next[v];
   if (u == -1) return dep[v]= 0;
   if (u == i) return dep[v]= N;
   return dep[v]= dfs(dfs, u) + 1;
  };
  ans= max(ans, dfs(dfs, i));
 }
 cout << (ans >= N ? -1 : ans) << '\n';
 return 0;
}