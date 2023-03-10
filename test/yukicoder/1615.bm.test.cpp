#define PROBLEM "https://yukicoder.me/problems/no/1615"
#include <iostream>
#include "src/Graph/BipartiteMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, K, L;
 cin >> N >> M >> K >> L;
 int X[L], Y[L], Z[L];
 for (int i= 0; i < L; ++i) cin >> X[i] >> Y[i] >> Z[i], --X[i], --Y[i], Z[i]= 1 << Z[i];
 long long ans= 0;
 for (int k= K + 1; k--;) {
  BipartiteMatching bm(N, M);
  int a= 1 << k;
  for (int i= L; i--;)
   if (Z[i] == a) bm.add_edge(X[i], Y[i]);
  bm.build();
  long long d= a - a / 2;
  ans+= d * bm.max_matching().size();
  for (int i= L; i--;) Z[i]-= d * (bm.used_as_vertex_cover_left(X[i]) + bm.used_as_vertex_cover_right(Y[i]));
 }
 cout << ans << '\n';
 return 0;
}