#define PROBLEM "https://atcoder.jp/contests/abc136/tasks/abc136_d"
#include <iostream>
#include <vector>
#include "src/FFT/BigInt.hpp"
#include "src/Graph/FunctionalGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 int N= S.length();
 FunctionalGraph graph(N);
 for (int i= 0; i < N; ++i)
  if (S[i] == 'L') graph.add_edge(i, i - 1);
  else graph.add_edge(i, i + 1);
 graph.build();
 BigInt K("1" + string(100, '0'));
 vector cnt(N, 0);
 for (int i= 0; i < N; ++i) ++cnt[graph.jump(i, K)];
 for (int i= 0; i < N; ++i) cout << cnt[i] << " \n"[i == N - 1];
 return 0;
}