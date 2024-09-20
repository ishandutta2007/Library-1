// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2644
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <string>
#include "src/DataStructure/SparseTable.hpp"
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 SuffixArray sa(S);
 SparseTable mn(sa.sa, [](int x, int y) { return min(x, y); });
 SparseTable mx(sa.sa, [](int x, int y) { return max(x, y); });
 int m;
 cin >> m;
 while (m--) {
  string x, y;
  cin >> x >> y;
  auto [lx, rx]= sa.pattern_matching(x);
  auto [ly, ry]= sa.pattern_matching(y);
  if (rx == lx || ry == ly) cout << 0 << '\n';
  else {
   int X= mn.prod(lx, rx), Y= mx.prod(ly, ry);
   if (X + x.length() > Y + y.length()) cout << 0 << '\n';
   else cout << Y - X + y.length() << '\n';
  }
 }
 return 0;
}
