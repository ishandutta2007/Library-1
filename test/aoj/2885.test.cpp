// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2885
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 2色 + 連結成分
#include <iostream>
#include <vector>
#include <bitset>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 for (int N, M; cin >> N >> M && N;) {
  UnionFind_Potentialized<bool> uf(N);
  bool isok= true;
  for (int i= 0; i < M; ++i) {
   int u, v;
   cin >> u >> v;
   isok&= uf.unite(--u, --v, 1);
  }
  if (!isok) {
   cout << 0 << '\n';
   continue;
  }
  vector<int> cnt(N);
  for (int i= 0; i < N; ++i) cnt[uf.leader(i)]+= uf.potential(i);
  bitset<1010> dp;
  dp[0]= 1;
  for (int i= 0; i < N; ++i) {
   if (i != uf.leader(i)) continue;
   dp= (dp << cnt[i]) | (dp << (uf.size(i) - cnt[i]));
  }
  vector<int> ans;
  for (int i= 0; i <= N; i+= 2)
   if (dp[i]) ans.push_back(i / 2);
  cout << ans.size() << '\n';
  for (int a: ans) cout << a << '\n';
 }
 return 0;
}