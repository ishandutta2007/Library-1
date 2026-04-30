// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1153
// competitive-verifier: TLE 5
// competitive-verifier: MLE 256
#include <iostream>
#include <vector>
#include <bitset>
#include "mylib/graph/Graph.hpp"
#include "mylib/graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 vector<int> A(M);
 for(auto& a: A) cin >> a, --a;
 Graph g(N, N - 1);
 for(int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];

 using U= bitset<1024>;
 auto put_edge= [&](int, int, int g_child) -> U {
  U b;
  b.set(g_child);
  return b;
 };
 auto op= [&](const U& l, const U& r) -> U { return l | r; };
 auto put_vertex= [&](int, const U& s) -> int {
  for(int i= 0;; ++i)
   if(!s.test(i)) return i;
 };
 Rerooting<int> rr(g, put_edge, op, U{}, put_vertex);

 int S= 0;
 for(int a: A) S^= rr[a];

 auto adjv= g.adjacency_vertex(0);
 for(int k= 0; k < M; ++k) {
  int u= A[k];
  for(int w: adjv[u])
   if((S ^ rr[u] ^ rr(u, w)) == 0) {
    cout << (k + 1) << ' ' << (w + 1) << '\n';
    return 0;
   }
 }
 cout << -1 << ' ' << -1 << '\n';
 return 0;
}
