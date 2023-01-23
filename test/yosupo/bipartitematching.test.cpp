#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"
#include <iostream>
#include "src/Graph/MatchingBipartite.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int L, R, M;
 cin >> L >> R >> M;
 MatchingBipartite graph(L, R);
 while (M--) {
  int a, b;
  cin >> a >> b;
  graph.add_edge(a, b);
 }
 auto ans= graph.get_matching();
 auto left= ans.second.first;
 cout << ans.first << '\n';
 for (int i= 0; i < (int)left.size(); i++)
  if (left[i] != -1) {
   cout << i << " " << left[i] << '\n';
  }
 return 0;
}