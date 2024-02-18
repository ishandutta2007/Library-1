#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/3032"
#include <iostream>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/general_matching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, A, B;
 cin >> N >> A >> B;
 Graph g(2 * N);
 int a[N], b[N];
 for (int i= 0; i < N; i++) cin >> a[i] >> b[i];
 for (int i= 0; i < N; i++) {
  if (abs(a[i] - b[i]) <= A || (B <= abs(a[i] - b[i]) && abs(a[i] - b[i]) <= 2 * A)) g.add_edge(i, i + N);
  for (int j= i + 1; j < N; j++) {
   int tmp= abs(a[i] + a[j] - b[i] - b[j]);
   if (tmp <= A || (B <= tmp && tmp <= 2 * A)) g.add_edge(i, j);
  }
 }
 auto [ans, _]= general_matching(g);
 cout << ans.size() << '\n';
 return 0;
}
