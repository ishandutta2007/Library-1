#define PROBLEM "https://yukicoder.me/problems/no/1293"
#include <iostream>
#include <algorithm>
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, D, W;
 cin >> N >> D >> W;
 StronglyConnectedComponents scc(N + N);
 for (int i= 0; i < D; ++i) {
  int a, b;
  cin >> a >> b;
  --a, --b;
  scc.add_edge(a, b), scc.add_edge(b, a);
 }
 for (int i= 0; i < W; ++i) {
  int c, d;
  cin >> c >> d;
  c+= N - 1, d+= N - 1;
  scc.add_edge(c, d), scc.add_edge(d, c);
 }
 for (int i= 0; i < N; ++i) scc.add_edge(i, i + N);
 scc.build();
 int C= scc.components_num();
 auto dag= scc.dag();
 long long dp[C];
 fill_n(dp, C, 0);
 for (int i= 0; i < N; ++i) ++dp[scc.belong(i)];
 for (int i= 0; i < C; ++i)
  for (int j: dag[i]) dp[j]+= dp[i];
 long long ans= 0;
 for (int i= 0; i < N; ++i) ans+= dp[scc.belong(i + N)] - 1;
 cout << ans << '\n';
 return 0;
}