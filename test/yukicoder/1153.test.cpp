#define PROBLEM "https://yukicoder.me/problems/no/1153"
#include <iostream>
#include <array>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 int A[M];
 for (int i= 0; i < M; ++i) cin >> A[i], --A[i];
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v;
  cin >> u >> v;
  tree.add_edge(--u, --v);
 }
 tree.build();
 auto mex= [](int v) -> int {
  for (int i= 0;; ++i)
   if (!((v >> i) & 1)) return i;
 };
 auto f_ee= [&](int l, int r) { return l | r; };
 auto f_ve= [&](int g, int, auto) { return 1 << g; };
 auto f_ev= [&](int d, int) { return mex(d); };
 auto dp= rerooting<int>(tree, f_ee, f_ve, f_ev, 0);
 int sum= 0;
 for (int i= 0; i < M; ++i) sum^= dp[A[i]];
 if (sum) {
  for (int i= 0; i < M; ++i) {
   int v= A[i], g= dp[v], h= sum ^ g;
   if (h < g)
    for (auto e: tree[v])
     if (int u= e.to; dp.get(v, u) == h) {
      cout << i + 1 << " " << u + 1 << '\n';
      return 0;
     }
  }
 } else cout << -1 << " " << -1 << '\n';
 return 0;
}