#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2644"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/DisjointSparseTable.hpp"
#include "src/String/SuffixArray.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);

  string s;
  int m;
  cin >> s >> m;
  SuffixArray sa(s);
  using P = pair<int, int>;
  auto f = [](P a, P b) {
    return P(max(a.first, b.first), min(a.second, b.second));
  };
  vector<P> vp;
  for (int i = 0; i < s.length(); i++) vp.emplace_back(sa[i], sa[i]);
  DisjointSparseTable<P> dst(vp, f);

  for (int i = 0; i < m; i++) {
    string x, y;
    cin >> x >> y;
    int lx = sa.lower_bound(x);
    int rx = sa.upper_bound(x);
    int ly = sa.lower_bound(y);
    int ry = sa.upper_bound(y);
    int ans;
    if (rx - lx <= 0 || ry - ly <= 0) {
      ans = 0;
    } else {
      int a = dst.query(ly, ry).first;
      int b = dst.query(lx, rx).second;
      if (b + (int)x.size() > a + (int)y.size())
        ans = 0;
      else
        ans = a - b + y.size();
    }
    cout << ans << "\n";
  }
  cout << flush;
  return 0;
}
