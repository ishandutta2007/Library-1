// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0342
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "src/DataStructure/UnionFind.hpp"
#include "src/Geometry/Convex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using Real= long double;
 using P= Point<Real>;
 int V, R;
 cin >> V >> R;
 vector<P> ps(V);
 map<P, int> id;
 for (int i= 0; i < V; ++i) cin >> ps[i], id[ps[i]]= i;
 UnionFind uf(V);
 Real ans= 0;
 for (const auto &e: Convex(ps).edges()) uf.unite(id[e.p], id[e.q]), ans+= e.length();
 vector<Segment<Real>> ss;
 for (int i= 0; i < R; ++i) {
  int s, t;
  cin >> s >> t;
  if (!uf.same(--s, --t)) ss.emplace_back(ps[s], ps[t]);
 }
 sort(ss.begin(), ss.end(), [](const auto &a, const auto &b) { return a.length() < b.length(); });
 for (const auto &e: ss)
  if (uf.unite(id[e.p], id[e.q])) ans+= e.length();
 cout << fixed << setprecision(12) << ans << '\n';
 return 0;
}