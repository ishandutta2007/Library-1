#define PROBLEM "https://judge.yosupo.jp/problem/set_xor_min"
#include <bits/stdc++.h>
#include "src/DataStructure/SegmentTree_Dynamic.hpp"
#include "src/DataStructure/BinaryTrie.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  BinaryTrie S;
  int Q;
  cin >> Q;
  while (Q--) {
    int op, x;
    cin >> op >> x;
    if (op == 0) {
      if (!S.count(x)) S.insert(x);
    } else if (op == 1) {
      if (S.count(x)) S.erase(x);
    } else {
      cout << (S.min_element(x) ^ x) << endl;
    }
  }
  return 0;
}