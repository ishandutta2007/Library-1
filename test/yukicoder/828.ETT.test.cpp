#define PROBLEM "https://yukicoder.me/problems/no/828"
#include <iostream>
#include <vector>
#include "src/DataStructure/EulerTourTree.hpp"
using namespace std;
struct RaddQ {
 using T= long long;
 using E= long long;
 static void mapping(T &t, E e, int sz) { t+= e; }
 static void composition(E &pre, E suf) { pre+= suf; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int r[N];
 for (int i= 0; i < N; i++) cin >> r[i];
 EulerTourTree<RaddQ> ett(N);
 vector<int> tree[N];
 for (int i= 0; i < N - 1; i++) {
  int u, v;
  cin >> u >> v;
  u--, v--;
  if (u > v) swap(u, v);
  ett.link(u, v);
  tree[v].push_back(u);
 }
 for (int v= N; v--;) {
  ett.apply_tree(v, 1);
  for (int u: tree[v]) ett.cut(u, v);
 }
 long long ans= 1;
 for (int i= 0; i < N; i++) (ans*= r[i] + ett[i])%= int(1e9 + 7);
 cout << ans << '\n';
 return 0;
}
