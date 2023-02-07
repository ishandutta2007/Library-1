#define PROBLEM "https://yukicoder.me/problems/no/768"
#include <iostream>
#include <vector>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v;
  cin >> u >> v;
  tree.add_edge(--u, --v);
 }
 tree.build();
 auto f_ee= [&](bool l, bool r) { return l | r; };
 auto f_ve= [&](bool d, int, auto) { return d; };
 auto f_ev= [&](bool d, int) { return !d; };
 auto dp= rerooting<bool>(tree, f_ee, f_ve, f_ev, false);
 vector<int> ans;
 for (int i= 0; i < N; ++i)
  if (dp[i]) ans.push_back(i + 1);
 cout << ans.size() << '\n';
 for (int x: ans) cout << x << '\n';
 return 0;
}