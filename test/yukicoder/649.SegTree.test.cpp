#define PROBLEM "https://yukicoder.me/problems/no/649"
#include <bits/stdc++.h>

#include "src/DataStructure/SegmentTree.hpp"
using namespace std;

struct RsumQ {
  using T = int;
  static T ti() { return 0; }
  static T op(const T &l, const T &r) { return l + r; }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int Q, K;
  cin >> Q >> K;
  K--;
  vector<long long> query;
  vector<long long> x;
  while (Q--) {
    long long v;
    cin >> v;
    if (v == 1) {
      cin >> v;
      x.push_back(v);
    } else {
      v = -1;
    }
    query.push_back(v);
  }
  sort(x.begin(), x.end());
  x.erase(unique(x.begin(), x.end()), x.end());
  SegmentTree<RsumQ> seg(x.size());
  for (auto q : query) {
    if (q < 0) {
      auto check = [&](int v) { return v <= K; };
      int i = seg.find_right(check);
      if (i < (int)x.size()) {
        cout << x[i] << endl;
        seg.set_val(i, seg[i] - 1);
      } else {
        cout << -1 << endl;
      }
    } else {
      int i = lower_bound(x.begin(), x.end(), q) - x.begin();
      seg.set_val(i, seg[i] + 1);
    }
  }
  return 0;
}
