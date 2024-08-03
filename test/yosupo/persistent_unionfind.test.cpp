// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/persistent_unionfind
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
   cout << uf[k].same(u, v) << endl;
  } else {
   uf[i]= uf[k];
   uf[i].unite(u, v);
  }
 }
}
