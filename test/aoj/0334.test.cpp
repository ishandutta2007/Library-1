#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0334"
#include <iostream>
#include "src/Graph/BipartiteMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N, --N;
 BipartiteMatching bm(N, N);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) {
   int x;
   cin >> x;
   if (x) bm.add_edge(j, i);
  }
 bm.build<1>();
 if (bm.max_matching().size() < N) {
  cout << "no" << '\n';
 } else {
  cout << "yes" << '\n';
  for (int i= 0; i < N; ++i) cout << bm.l_to_r(i) + 1 << '\n';
 }
 return 0;
}