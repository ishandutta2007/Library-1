#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2559"
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "src/DataStructure/LinkCutTree.hpp"
#include "src/DataStructure/UnionFind.hpp"
using namespace std;
struct RchminQ {
 using T= long long;
 using E= long long;
 static void mapping(T &v, const E &f) {
  if (v > f) v= f;
 }
 static void composition(E &pre, const E &suf) {
  if (pre > suf) pre= suf;
 }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, m;
 cin >> n >> m;
 UnionFind uf(n);
 int a[m], b[m];
 long long w[m];
 for (int i= 0; i < m; i++) {
  cin >> a[i] >> b[i] >> w[i];
  --a[i], --b[i];
 }
 int ord[m];
 iota(ord, ord + m, 0), sort(ord, ord + m, [&](int l, int r) { return w[l] < w[r]; });
 vector<int> mst_es;
 long long mst_cost= 0;
 for (int i: ord)
  if (uf.unite(a[i], b[i])) mst_cost+= w[i], mst_es.push_back(i);

 static constexpr int INF= 1 << 30;
 LinkCutTree<RchminQ> lct(2 * n - 1, INF);
 int id[m];
 fill_n(id, m, -1);
 int num= n;
 for (int i: mst_es) id[i]= num++, lct.link(a[i], id[i]), lct.link(id[i], b[i]);
 for (int i= 0; i < m; ++i)
  if (id[i] == -1) lct.apply(a[i], b[i], w[i]);
 long long ans[m];
 for (int i= 0; i < m; ++i) {
  if (id[i] == -1) ans[i]= mst_cost;
  else {
   long long tmp= lct[id[i]];
   ans[i]= tmp == INF ? -1 : mst_cost - w[i] + tmp;
  }
 }
 for (auto a: ans) cout << a << '\n';
 return 0;
}