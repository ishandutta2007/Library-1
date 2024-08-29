// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3198
// competitive-verifier: TLE 1.5

// 推論補助のtest (しなくても通るが...)
// 推論補助しない場合 (最悪ケースで) 0.5s 程度遅くなる

#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/general_matching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N + N, M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i], g[i].second+= N;
 int Q;
 cin >> Q;
 std::vector<int> match, partner(N + N, -1);
 while (Q--) {
  int x, y;
  cin >> x >> y, --x, --y, y+= N;
  auto it= std::find(g.begin(), g.end(), make_pair(x, y));
  if (it != g.end()) {
   g.erase(it);
   if (partner[x] == y) partner[x]= partner[y]= -1;
  } else g.emplace_back(x, y);
  tie(match, partner)= general_matching(g, partner);
  cout << (match.size() == N ? "Yes" : "No") << '\n';
 }
 return 0;
}