#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Algorithm/Mo.hpp"
#include "src/DataStructure/BinaryIndexedTree.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  int A[N];
  for (int i = 0; i < N; i++) cin >> A[i];
  vector<int> v(A, A + N);
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  for (int i = 0; i < N; i++)
    A[i] = lower_bound(v.begin(), v.end(), A[i]) - v.begin();
  Mo mo(N);
  for (int q = 0; q < Q; q++) {
    int l, r;
    cin >> l >> r;
    mo.query(l, r);
  }
  BinaryIndexedTree<int> bit(v.back() + 10);
  int inv = 0, total = 0;
  auto addl = [&](int i) {
    inv += bit.sum(A[i]);
    bit.add(A[i], 1);
    total++;
  };
  auto addr = [&](int i) {
    inv += total - bit.sum(A[i] + 1);
    bit.add(A[i], 1);
    total++;
  };
  auto erasel = [&](int i) {
    inv -= bit.sum(A[i]);
    bit.add(A[i], -1);
    total--;
  };
  auto eraser = [&](int i) {
    inv -= total - bit.sum(A[i] + 1);
    bit.add(A[i], -1);
    total--;
  };
  int ans[Q];
  auto out = [&](int q) { ans[q] = inv; };
  mo.run(addl, addr, erasel, eraser, out);
  for (int q = 0; q < Q; q++) cout << ans[q] << "\n";
  cout << flush;
  return 0;
}