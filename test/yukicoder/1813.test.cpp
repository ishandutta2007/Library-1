#define PROBLEM "https://yukicoder.me/problems/no/1813"
#include <iostream>
#include <algorithm>
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 StronglyConnectedComponents scc(N);
 for (int i= 0; i < M; ++i) {
  int A, B;
  cin >> A >> B;
  scc.add_edge(--A, --B);
 }
 auto blk= scc.get_block();
 int C= blk.size();
 if (C == 1) {
  cout << 0 << '\n';
 } else {
  auto dag= scc.get_dag(scc.get_index(blk), C);
  int cnt[2]= {0, 0};
  bool st[C];
  fill_n(st, C, true);
  for (int i= 0; i < C; ++i) {
   if (st[i]) ++cnt[0];
   for (int j: dag[i]) st[j]= false;
   if (!dag[i].size()) ++cnt[1];
  }
  cout << max(cnt[0], cnt[1]) << '\n';
 }
 return 0;
}