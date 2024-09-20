// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/persistent_unionfind
// competitive-verifier: TLE 1
// competitive-verifier: MLE 1024
#include <iostream>
#include <vector>
#include "src/DataStructure/PersistentArray.hpp"
#include "src/DataStructure/UnionFind_Persistent.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<UnionFind_Persistent> uf(Q + 1);
 uf[0]= UnionFind_Persistent(N);
 for (int i= 1; i <= Q; i++) {
  int t, k, u, v;
  cin >> t >> k >> u >> v;
  k++;
  if (t) {
   cout << uf[k].connected(u, v) << '\n';
  } else {
   uf[i]= uf[k];
   uf[i].unite(u, v);
  }
 }
}
