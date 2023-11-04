#define PROBLEM "https://yukicoder.me/problems/no/1216"
#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include "src/DataStructure/SegmentTree_2D.hpp"
#include "src/Graph/Tree.hpp"
using namespace std;
struct RSQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Tree<long long, true> tree(N);
 for (int i= 1; i < N; ++i) {
  int A, B;
  long long C;
  cin >> A >> B >> C;
  tree.add_edge(--A, --B, C);
 }
 tree.build(0);
 set<array<long long, 2>> st;
 vector<tuple<int, int, int, long long>> query;
 for (int i= 0; i < Q; ++i) {
  int tp, v;
  long long t, l;
  cin >> tp >> v >> t >> l, --v;
  if (tp == 0) {
   long long x= tree.to_seq(v), y= t + tree.depth_w(v);
   query.emplace_back(1, 0, x, y);
   st.insert({x, y});
   if (int u= tree.parent(tree.la_w(v, l)); u != -1) {
    x= tree.to_seq(u);
    query.emplace_back(-1, 0, x, y);
    st.insert({x, y});
   }
  } else {
   auto [l, r]= tree.subtree(v);
   query.emplace_back(0, l, r, t + tree.depth_w(v));
  }
 }
 SegmentTree_2D<long long, RSQ> seg(st);
 for (auto [t, a, b, y]: query) {
  if (t == 0) cout << seg.fold(a, b, 0, y + 1) << '\n';
  else seg.mul(b, y, t);
 }
 return 0;
}
