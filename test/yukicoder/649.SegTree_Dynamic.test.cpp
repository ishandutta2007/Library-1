#define PROBLEM "https://yukicoder.me/problems/no/649"
#include <bits/stdc++.h>
#include "src/DataStructure/SegmentTree_Dynamic.hpp"
using namespace std;

struct RsumQ {
  using T = int;
  static T ti() { return 0; }
  static T f(const T &l, const T &r) { return l + r; }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int Q, K;
  cin >> Q >> K;
  K--;
  SegmentTree_Dynamic<RsumQ> seg((long long)1e18 + 10);
  while (Q--) {
    long long v;
    cin >> v;
    if (v == 1) {
      cin >> v;
      seg.set_val(v, seg[v] + 1);
    } else {
      auto check = [&](int x) { return x > K; };
      v = seg.find_first(check);
      cout << v << endl;
      if (v >= 0) seg.set_val(v, seg[v] - 1);
    }
  }
  return 0;
}