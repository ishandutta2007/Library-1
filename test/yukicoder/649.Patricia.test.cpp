// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/649
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/SegmentTree_Patricia.hpp"
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
 SegmentTree_Patricia<RsumQ, false, 60> seg;
 while (Q--) {
  long long v;
  cin >> v;
  if (v == 1) {
   cin >> v;
   seg.set(v, seg[v] + 1);
  } else {
   auto check= [&](int x) { return x >= K; };
   v= seg.find_first(0, check);
   cout << v << '\n';
   if (v >= 0) seg.set(v, seg[v] - 1);
  }
 }
 return 0;
}