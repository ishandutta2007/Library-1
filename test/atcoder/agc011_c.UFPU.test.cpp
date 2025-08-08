// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/agc011/tasks/agc011_c
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF (2部グラフ判定+連結成分)
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized_Undoable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 UnionFind_Potentialized_Undoable<bool> uf(N);
 vector<int> b(N, 1);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v, --u, --v;
  int x= uf.leader(u), y= uf.leader(v);
  b[x]= b[y]= b[x] & b[y];
  if (!uf.unite(u, v, 1)) b[x]= b[y]= 0;
 }
 long long one= 0, bin= 0, oth= 0;
 vector<int> used(N, 0);
 for (int i= 0; i < N; ++i) {
  int v= uf.leader(i);
  if (used[v]) continue;
  used[v]= 1;
  if (uf.size(v) == 1) ++one;
  else if (b[v]) ++bin;
  else ++oth;
 }
 cout << one * N * 2 - one * one + bin * bin * 2 + bin * oth * 2 + oth * oth << '\n';
 return 0;
}