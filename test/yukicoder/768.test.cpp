// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/768
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 auto put_edge= [&](int, int, bool d) { return d; };
 auto op= [&](bool l, bool r) { return l | r; };
 auto put_vertex= [&](int, bool d) { return !d; };
 Rerooting<bool> dp(g, put_edge, op, false, put_vertex);
 vector<int> ans;
 for (int i= 0; i < N; ++i)
  if (dp[i]) ans.push_back(i + 1);
 cout << ans.size() << '\n';
 for (int x: ans) cout << x << '\n';
 return 0;
}