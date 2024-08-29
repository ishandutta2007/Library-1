// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2893
// competitive-verifier: TLE 2
// 非想定解 MLがギリギリ
#include <iostream>
#include "src/DataStructure/OnlineDynamicConnectivity.hpp"
using namespace std;
struct Sum {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 int u[M], v[M];
 long long w[M];
 OnlineDynamicConnectivity<Sum> dicon(N);
 for (int i= 0; i < M; ++i) {
  cin >> u[i] >> v[i] >> w[i];
  --u[i], --v[i];
  dicon.link(u[i], v[i]);
  dicon.set(u[i], dicon[u[i]] + w[i]);
  dicon.set(v[i], dicon[v[i]] + w[i]);
 }
 long long min_score= 1ll << 60;
 long long ans_u= 0, ans_v= 0;
 for (int i= 0; i < M; ++i) {
  dicon.cut(u[i], v[i]);
  long long score;
  if (dicon.connected(u[i], v[i])) {
   score= dicon.prod(u[i]) / 2 - w[i];
  } else {
   long long WA= dicon.prod(u[i]);
   long long WB= dicon.prod(v[i]);
   score= abs(WA - WB) / 2;
  }
  if (min_score > score) min_score= score, ans_u= u[i], ans_v= v[i];
  else if (min_score == score) {
   if (ans_u > u[i]) ans_u= u[i], ans_v= v[i];
   else if (ans_u == u[i] && ans_v > v[i]) ans_v= v[i];
  }
  dicon.link(u[i], v[i]);
 }
 cout << ans_u + 1 << " " << ans_v + 1 << '\n';
 return 0;
}