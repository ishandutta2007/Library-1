// competitive-verifier: PROBLEM https://atcoder.jp/contests/utpc2013/tasks/utpc2013_11
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Graph/BipartiteGraph.hpp"
#include "src/Graph/DulmageMendelsohn.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, m;
 cin >> n >> m;
 vector<int> ord(n + n), dro(n + n);
 for (int i= 0; i < n + n; ++i) ord[i]= i & 1 ? n + i / 2 : i / 2, dro[ord[i]]= i;
 BipartiteGraph bg(n, n);
 for (int i= 0; i < m; ++i) {
  int a, b, f;
  cin >> a >> b >> f, --a, --b;
  bg.add_edge(ord[a], ord[b]);
 }
 DulmageMendelsohn dm(bg);
 auto ans= dm.min_vertex_cover(ord);
 cout << ans.size() << '\n';
 for (int x: ans) cout << dro[x] + 1 << '\n';
 return 0;
}