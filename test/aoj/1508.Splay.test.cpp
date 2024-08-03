// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1508

// split3, push_front, pop_back, operator+, prod, set の verify

#include <iostream>
#include "src/DataStructure/SplayTree.hpp"
using namespace std;
struct RminQ {
 using T= int;
 static T op(T l, T r) { return min(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 int a[n];
 for (int i= 0; i < n; i++) cin >> a[i];
 SplayTree<RminQ> st(a, a + n);
 while (q--) {
  int x, y, z;
  cin >> x >> y >> z;
  if (x == 0) {
   auto [l, c, r]= st.split3(y, z + 1);
   c.push_front(c.pop_back());
   st= l + c + r;
  } else if (x == 1) cout << st.prod(y, z + 1) << '\n';
  else st.set(y, z);
 }
 return 0;
}