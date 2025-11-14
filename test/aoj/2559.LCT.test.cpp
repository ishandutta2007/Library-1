// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2559
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 双対

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "src/DataStructure/LinkCutTree.hpp"
#include "src/DataStructure/UnionFind.hpp"
using namespace std;
struct RchminQ {
 using T= int;
 using E= int;
 static void mp(T &v, const E &f) {
  if (v > f) v= f;
 }
 static void cp(E &pre, const E &suf) {
  if (pre > suf) pre= suf;
 }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, m;
 cin >> n >> m;
 UnionFind uf(n);
 int a[m], b[m], w[m];
 for (int i= 0; i < m; i++) cin >> a[i] >> b[i] >> w[i], --a[i], --b[i];
 vector<int> mst_es;
 long long mst_cost= 0;
 {
  int ord[m];
  iota(ord, ord + m, 0), sort(ord, ord + m, [&](int l, int r) { return w[l] < w[r]; });
  for (int i: ord)
   if (uf.unite(a[i], b[i])) mst_cost+= w[i], mst_es.push_back(i);
 }
 static constexpr int INF= 1 << 30;
 LinkCutTree<RchminQ> lct(2 * n - 1, INF);
 int id[m];
 fill_n(id, m, -1);
 int num= n;
 for (int i: mst_es) id[i]= num++, lct.link(a[i], id[i]), lct.link(id[i], b[i]);
 for (int i= 0; i < m; ++i)
  if (id[i] == -1) lct.apply(a[i], b[i], w[i]);
 for (int i= 0; i < m; ++i) {
  if (id[i] == -1) cout << mst_cost << '\n';
  else {
   long long tmp= lct[id[i]];
   cout << (tmp == INF ? -1 : mst_cost - w[i] + tmp) << '\n';
  }
 }
 return 0;
}