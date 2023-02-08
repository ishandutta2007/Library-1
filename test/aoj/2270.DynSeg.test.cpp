#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/VPC/UTPC/2270"
// 永続化 + find * 4 の verify
// tree の par verify
#include <iostream>
#include <vector>
#include "src/DataStructure/SegmentTree_Dynamic.hpp"
#include "src/Graph/Tree.hpp"
using namespace std;
struct RsumQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T vl, T vr) { return vl + vr; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 int x[N + 1];
 for (int i= 1; i <= N; i++) cin >> x[i];
 Tree tree(N + 1);
 vector<int> adj[N + 1];
 tree.add_edge(1, 0);
 for (int i= 0; i < N - 1; i++) {
  int a, b;
  cin >> a >> b;
  tree.add_edge(a, b), adj[a].push_back(b), adj[b].push_back(a);
 }
 tree.build(0);
 using Seg= SegmentTree_Dynamic<RsumQ, true>;
 Seg segs[N + 1];
 auto dfs= [&adj, &segs, &x](auto f, int v, int p) -> void {
  segs[v]= segs[p];
  segs[v].set(x[v], segs[v][x[v]] + 1);
  for (int u: adj[v])
   if (u != p) f(f, u, v);
 };
 dfs(dfs, 1, 0);
 for (int i= 0; i < Q; i++) {
  int v, w, l;
  cin >> v >> w >> l;
  auto check= [&](int x, int y, int a, int ap) { return x + y - a - ap >= l; };
  int lca= tree.lca(v, w), lcap= tree.parent(lca);
  cout << Seg::find_first<4>(0, check, {segs[v], segs[w], segs[lca], segs[lcap]}) << '\n';
 }
 return 0;
}