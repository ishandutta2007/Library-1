// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/274
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF(無向2SAT)
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 int L[N], R[N];
 for (int i= 0; i < N; ++i) cin >> L[i] >> R[i];
 UnionFind_Potentialized<bool> uf(N);
 for (int i= N; i--;)
  for (int j= i; j--;) {
   if (!(R[i] < L[j] || R[j] < L[i]))
    if (!uf.unite(i, j, 1)) return cout << "NO" << '\n', 0;
   if (!(R[i] < M - 1 - R[j] || M - 1 - L[j] < L[i]))
    if (!uf.unite(i, j, 0)) return cout << "NO" << '\n', 0;
  }
 cout << "YES" << '\n';
 return 0;
}