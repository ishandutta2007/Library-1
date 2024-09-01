// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/lesson/8/ITP2/all/ITP2_2_D
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// push_back, operator+= の verify

#include <iostream>
#include "src/DataStructure/SplayTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 SplayTree<int> ar[n];
 for (int i= 0; i < q; ++i) {
  int op;
  cin >> op;
  if (op == 0) {
   int t, x;
   cin >> t >> x;
   ar[t].push_back(x);
  } else if (op == 1) {
   int t;
   cin >> t;
   auto ans= ar[t].dump();
   for (int i= 0; i < ans.size(); ++i) cout << (i ? " " : "") << ans[i];
   cout << '\n';
  } else {
   int s, t;
   cin >> s >> t;
   ar[t]+= ar[s];
   ar[s].clear();
  }
 }
 return 0;
}