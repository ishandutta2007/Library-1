// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3198
// competitive-verifier: TLE 1.5

// 推論補助のtest (しなくても通るが...)
// 推論補助しない場合 (最悪ケースで) 0.7s 程度遅くなる

#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/BipartiteGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 BipartiteGraph bg(N, N, M);
 for (int i= 0; i < M; ++i) cin >> bg[i], --bg[i], bg[i].second+= N;
 int Q;
 cin >> Q;
 std::vector<int> match, partner(N + N, -1);
 while (Q--) {
  int x, y;
  cin >> x >> y, --x, --y, y+= N;
  auto it= std::find(bg.begin(), bg.end(), make_pair(x, y));
  if (it != bg.end()) {
   bg.erase(it);
   if (partner[x] == y) partner[x]= partner[y]= -1;
  } else bg.emplace_back(x, y);
  tie(match, partner)= bipartite_matching(bg, partner);
  cout << (match.size() == N ? "Yes" : "No") << '\n';
 }
 return 0;
}
