// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_add_range_sum
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/DataStructure/BinaryIndexedTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 BinaryIndexedTree<long long> bit(N);
 for (int i= 0; i < N; i++) {
  long long a;
  cin >> a;
  bit.add(i, a);
 }
 while (Q--) {
  int t, a, b;
  cin >> t >> a >> b;
  if (t) cout << bit.sum(a, b) << '\n';
  else bit.add(a, b);
 }
}
