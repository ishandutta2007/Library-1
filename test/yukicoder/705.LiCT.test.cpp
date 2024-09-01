// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/705
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <algorithm>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 long long a[n], x[n], y[n];
 for (int i= 0; i < n; ++i) cin >> a[i];
 for (int i= 0; i < n; ++i) cin >> x[i];
 for (int i= 0; i < n; ++i) cin >> y[i];
 auto f= [](long long x) { return abs(x * x * x); };
 auto w= [&](int i, int j, long long d) { return d + f(x[j] - a[i - 1]) + f(y[j]); };
 LiChaoTree lct(w, 1, n + 1);
 auto tree= lct.make_tree<MINIMIZE>();
 tree.insert(0, 0);
 for (int i= 1; i < n; ++i) {
  auto dp= tree.query(i).first;
  tree.insert(i, dp);
 }
 cout << tree.query(n).first << '\n';
 return 0;
}