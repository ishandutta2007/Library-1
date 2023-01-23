#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_A"
#include <iostream>
#include <algorithm>
#include "src/Graph/ReRooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 auto op= [](long long l, long long r) { return max(l, r); };
 auto lift= [](long long l, long long dat) { return l + dat; };
 ReRooting<long long, long long> tree(n, op, 0, lift);
 for (int i= 0; i < n - 1; i++) {
  int s, t;
  long long w;
  cin >> s >> t >> w;
  tree.add_edge(s, t, w);
  tree.add_edge(t, s, w);
 }
 auto ret= tree.run();
 long long ans= 0;
 for (int i= 0; i < n; i++) ans= max(ans, ret[i]);
 cout << ans << '\n';
 return 0;
}