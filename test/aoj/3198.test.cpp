#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/3198"
// eraseのverify
#include <iostream>
#include <set>
#include <array>
#include "src/Graph/BipartiteMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 BipartiteMatching bm(N, N);
 set<array<int, 2>> pos;
 for (int i= 0; i < M; ++i) {
  int a, b;
  cin >> a >> b, --a, --b;
  pos.insert({a, b}), bm.add_edge(a, b);
 }
 int Q;
 cin >> Q;
 while (Q--) {
  int x, y;
  cin >> x >> y, --x, --y;
  if (pos.count({x, y})) pos.erase({x, y}), bm.erase_edge(x, y);
  else pos.insert({x, y}), bm.add_edge(x, y);
  bm.build();
  cout << (bm.max_matching().size() == N ? "Yes" : "No") << '\n';
 }
 return 0;
}