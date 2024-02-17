#define PROBLEM "https://atcoder.jp/contests/abc202/tasks/abc202_e"
#include <iostream>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N);
 for (int i= 1, p; i < N; ++i) cin >> p, g.add_edge(p - 1, i);
 HeavyLightDecomposition tree(g, 0);
 vector<int> d(N);
 for (int v= N; v--;) d[tree.to_seq(v)]= tree.depth(v);
 WaveletMatrix wm(d);
 int Q;
 cin >> Q;
 while (Q--) {
  int U, D;
  cin >> U >> D;
  auto [l, r]= tree.subtree(U - 1);
  cout << wm.count(l, r, D, D + 1) << '\n';
 }
 return 0;
}