// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc328/tasks/abc328_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized_Undoable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 UnionFind_Potentialized_Undoable<long long> uf(N);
 vector<int> S;
 for (int i= 0; i < Q; ++i) {
  int a, b, d;
  cin >> a >> b >> d, --a, --b;
  if (uf.unite(a, b, d)) S.push_back(i + 1);
 }
 for (int i= 0, m= S.size(); i < m; ++i) cout << S[i] << " \n"[i + 1 == m];
 return 0;
}