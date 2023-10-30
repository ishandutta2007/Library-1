#define PROBLEM "https://atcoder.jp/contests/abc202/tasks/abc202_e"
#include <iostream>
#include <vector>
#include "src/Graph/Tree.hpp"
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 1; i < N; ++i) {
  int p;
  cin >> p;
  tree.add_edge(p - 1, i);
 }
 tree.build(0);
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