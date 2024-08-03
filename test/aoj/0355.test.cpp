// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/0355

// ロリハをセグ木で表現

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/Misc/rng.hpp"
#include "src/DataStructure/SegmentTree_Beats.hpp"
using namespace std;
using Mint= ModInt<int(1e9) + 7>;
using K= Pointwise<Mint, Mint>;
K bs, iv;
K pw[200010];
void init(int n) {
 bs= {rng(), rng()};
 iv= K(1) / (bs - 1);
 pw[0]= 1;
 for (int i= 1; i <= n; ++i) pw[i]= pw[i - 1] * bs;
}
struct M {
 struct T {
  K val;
  int sz;
 };
 using E= char;
 static T ti() { return {0, 0}; }
 static T op(const T& l, const T& r) { return {l.val * pw[r.sz] + r.val, l.sz + r.sz}; }
 static bool mp(T& v, E c) {
  v.val= (pw[v.sz] - 1) * iv * c;
  return true;
 }
 static void cp(E& f, E g) { f= g; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 init(N);
 string U;
 cin >> U;
 vector<M::T> A(N);
 for (int i= 0; i < N; ++i) A[i]= {U[i], 1};
 SegmentTree_Beats<M> seg(A);
 int Q;
 cin >> Q;
 while (Q--) {
  string op;
  cin >> op;
  if (op == "set") {
   int x, y;
   char z;
   cin >> x >> y >> z, --x;
   seg.apply(x, y, z);
  } else {
   int a, b, c, d;
   cin >> a >> b >> c >> d, --a, --c;
   int s= b - a, t= d - c, m= min(s, t);
   int ok= 0, ng= m + 1;
   while (ng - ok > 1) {
    int x= (ok + ng) / 2;
    if (seg.fold(a, a + x).val == seg.fold(c, c + x).val) ok= x;
    else ng= x;
   }
   if (ok == m) {
    cout << (s < t ? "s" : s > t ? "t" : "e") << '\n';
   } else {
    K sck= seg[a + ok].val, tck= seg[c + ok].val;
    char sc= std::get<0>(sck).val(), tc= std::get<0>(tck).val();
    cout << (sc < tc ? "s" : "t") << '\n';
   }
  }
 }
 return 0;
}