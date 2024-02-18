#define PROBLEM "https://yukicoder.me/problems/no/1293"
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, D, W;
 cin >> N >> D >> W;
 Graph g(N + N);
 for (int i= 0, a, b; i < D; ++i) cin >> a >> b, --a, --b, g.add_edge(a, b), g.add_edge(b, a);
 for (int i= 0, c, d; i < W; ++i) cin >> c >> d, c+= N - 1, d+= N - 1, g.add_edge(c, d), g.add_edge(d, c);
 for (int i= 0; i < N; ++i) g.add_edge(i, i + N);
 StronglyConnectedComponents scc(g);
 int C= scc.size();
 auto dag= scc.dag(g).adjacency_vertex(1);
 vector<long long> dp(C);
 for (int i= 0; i < N; ++i) ++dp[scc(i)];
 for (int i= 0; i < C; ++i)
  for (int j: dag[i]) dp[j]+= dp[i];
 long long ans= 0;
 for (int i= 0; i < N; ++i) ans+= dp[scc(i + N)] - 1;
 cout << ans << '\n';
 return 0;
}