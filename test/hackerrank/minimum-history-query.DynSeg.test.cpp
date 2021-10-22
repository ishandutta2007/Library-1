#define PROBLEM                                                         \
  "https://www.hackerrank.com/contests/happy-query-contest/challenges/minimum-history-query"

// 永続化 の verify

#include <bits/stdc++.h>
#include "src/DataStructure/DynamicSegmentTree.hpp"
using namespace std;

struct RMQ {
  using T = int;
  static T op(T l, T r) { return min(l, r); }
  static T ti() { return INT_MAX; }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  int A[N];
  for (int i = 0; i < N; i++) cin >> A[i];
  int Q1;
  cin >> Q1;
  using Seg = DynamicSegmentTree<RMQ, true>;
  Seg seg[Q1 + 1] = {Seg(A, A + N)};
  for (int i = 1; i <= Q1; i++) {
    int p, x;
    cin >> p >> x;
    seg[i] = seg[i - 1];
    seg[i].set(--p, x);
  }
  int Q2;
  cin >> Q2;
  int X = 0;
  for (int i = 0; i < Q2; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    int k = X ^ a, l = b ^ X, r = c ^ X;
    cout << (X = seg[k].fold(--l, r));
    if (i < Q2 - 1) cout << '\n';
  }
  return 0;
}