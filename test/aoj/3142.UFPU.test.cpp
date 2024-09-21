// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3142
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

// 接続行列の連立方程式 (long long)
// ポテンシャルUF
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/incidence_matrix_equation.hpp"
#include "src/DataStructure/UnionFind_Potentialized_Undoable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 vector<long long> c(N);
 for (int i= 0; i < N; ++i) cin >> c[i];
 for (int i= 0, b; i < N; ++i) cin >> b, c[i]-= b;
 auto sol= incidence_matrix_equation(g, c);
 UnionFind_Potentialized_Undoable<long long> uf(N);
 for (int i= 0; i < N - 1; ++i) {
  auto [u, v]= g[i];
  uf.unite(v, u, sol[i]);
 }
 long long sum= 0, lo= 0;
 for (int i= 0; i < N; ++i) {
  long long p= uf.potential(i);
  sum+= p, lo= min(lo, p);
 }
 cout << sum - lo * N << '\n';
 return 0;
}