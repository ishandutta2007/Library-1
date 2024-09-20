// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2294
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/UnionFind_Potentialized_Undoable.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Math/Nimber.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 Nimber::init();
 int N, X, Q;
 cin >> N >> X >> Q;
 UnionFind_Potentialized_Undoable<Nimber> uf(N);
 vector<array<int, 30>> cnt(N);
 while (Q--) {
  int t;
  cin >> t;
  if (t == 1) {
   int v, w;
   cin >> v >> w;
   int a= uf.leader(X), b= uf.leader(v);
   int as= uf.size(X), bs= uf.size(v);
   w^= uf.diff(v, X).val();
   uf.unite(a, b, w);
   if (a != uf.leader(a)) swap(a, b), swap(as, bs);
   for (int i= 30; i--;) {
    if ((w >> i) & 1) cnt[a][i]+= bs - cnt[b][i];
    else cnt[a][i]+= cnt[b][i];
   }
  } else if (t == 2) {
   int u, v;
   cin >> u >> v;
   if (uf.connected(u, v)) {
    int ans= uf.diff(u, v).val();
    cout << ans << '\n';
    X+= ans;
    X%= N;
   } else cout << -1 << '\n';
  } else if (t == 3) {
   int v;
   cin >> v;
   v= uf.leader(v);
   Mint ans= 0;
   for (int i= 30; i--;) ans+= Mint(cnt[v][i]) * (uf.size(v) - cnt[v][i]) * (1 << i);
   cout << ans << '\n';
  } else {
   int value;
   cin >> value;
   X+= value;
   X%= N;
  }
 }
 return 0;
}