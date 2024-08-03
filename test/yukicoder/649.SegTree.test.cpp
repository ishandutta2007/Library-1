// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/649
#include <iostream>
#include "src/Misc/compress.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct RsumQ {
 using T= int;
 static T ti() { return 0; }
 static T op(const T &l, const T &r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q, K;
 cin >> Q >> K;
 vector<long long> query, x;
 while (Q--) {
  long long v;
  cin >> v;
  if (v == 1) {
   cin >> v;
   x.push_back(v);
  } else {
   v= -1;
  }
  query.push_back(v);
 }
 auto id= compress(x);
 SegmentTree<RsumQ> seg(x.size());
 for (auto q: query) {
  if (q < 0) {
   auto check= [&](int v) { return v >= K; };
   int i= seg.find<0>(check, 0);
   if (i != -1) {
    cout << x[i] << endl;
    seg.set(i, seg[i] - 1);
   } else {
    cout << -1 << endl;
   }
  } else {
   int i= id(q);
   seg.set(i, seg[i] + 1);
  }
 }
 return 0;
}
