// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1270
// competitive-verifier: TLE 1.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/SegmentTree_Beats.hpp"
#include "src/DataStructure/BinaryIndexedTree.hpp"
#include "src/Misc/Mo.hpp"
using namespace std;
struct RmQRaQ {
 using T= int;
 using E= int;
 static T ti() { return 1 << 30; }
 static T op(T l, T r) { return min(l, r); }
 static bool mp(T &v, E x) {
  if (v != ti()) v+= x;
  return true;
 }
 static void cp(E &p, E s) { p+= s; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<int> a(N);
 for (int i= 0; i < N; ++i) cin >> a[i], --a[i];
 BinaryIndexedTree<int> bitl(N), bitr(N);
 SegmentTree_Beats<RmQRaQ> seg(N, 0);
 int sum= 0, sz= 0;
 for (auto x: a) {
  sum+= bitr.sum(x + 1, N);
  bitr.add(x, 1);
  seg.apply(x + 1, N, 1);
 }
 Mo mo;
 for (int q= 0; q < Q; ++q) {
  int l, r;
  cin >> l >> r;
  mo.query(--l, r);
 }
 auto addl= [&](int i) {
  int x= a[i];
  sum-= bitl.sum(x + 1, N) + bitr.sum(0, x);
  bitl.add(x, -1);
  seg.apply(0, x, -1);
  ++sz;
 };
 auto addr= [&](int i) {
  int x= a[i];
  sum-= bitr.sum(0, x) + bitl.sum(x + 1, N);
  bitr.add(x, -1);
  seg.apply(x + 1, N, -1);
  ++sz;
 };
 auto erasel= [&](int i) {
  int x= a[i];
  sum+= bitl.sum(x + 1, N) + bitr.sum(0, x);
  bitl.add(x, 1);
  seg.apply(0, x, 1);
  --sz;
 };
 auto eraser= [&](int i) {
  int x= a[i];
  sum+= bitr.sum(0, x) + bitl.sum(x + 1, N);
  bitr.add(x, 1);
  seg.apply(x + 1, N, 1);
  --sz;
 };
 vector<int> ans(Q);
 auto out= [&](int q) { ans[q]= sum + seg.prod(0, N) * sz; };
 mo.run(addl, addr, erasel, eraser, out);
 for (auto x: ans) cout << x << '\n';
 return 0;
}